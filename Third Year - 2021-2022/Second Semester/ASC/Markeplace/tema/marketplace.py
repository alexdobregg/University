"""
This module represents the Marketplace.

Computer Systems Architecture Course
Assignment 1
March 2023
"""
from threading import Lock, current_thread
import logging
from logging.handlers import RotatingFileHandler
import time
import unittest
from .product import Coffee, Tea

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S %Z',
    handlers=[
        RotatingFileHandler('marketplace.log', maxBytes=1000000, backupCount=1000)
    ]
)

logging.Formatter.converter = time.gmtime

class Marketplace:
    """
    Class that represents the Marketplace. It's the central part of the implementation.
    The producers and consumers use its methods concurrently.
    """
    def __init__(self, queue_size_per_producer):
        """
        Constructor
        Defined the dictionaries for producers, carts, ids and
        the Locks. Used 3 locks in total for this implementation.
        :type queue_size_per_producer: Int
        :param queue_size_per_producer: the maximum size of a queue associated with each producer
        """
        logging.info('Entered init - Queue size per producer: %d', queue_size_per_producer)
        self.producer_max_queue_size = queue_size_per_producer
        self.producers = {}
        self.ids = {"producer_ids": 0, "cart_ids": 0}
        self.carts = {}

        self.register_producer_lock = Lock()
        self.new_cart_lock = Lock()
        self.print_lock = Lock()
        logging.info('Left init')

    def register_producer(self):
        """
        Returns an id for the producer that calls this.
        Since increment is not atomic, we need a lock to make sure
        that each producer has its own id and they do not increment
        another producer's id.
        """
        logging.info('Entered register_producer')
        with self.register_producer_lock:
            current_id = self.ids["producer_ids"]
            self.producers[current_id] = []
            self.ids["producer_ids"] += 1
        logging.info('Left register_producer')
        return current_id

    def publish(self, producer_id, product):
        """
        Adds the product provided by the producer to the marketplace.
        Since we know exactly which producer will publish the id, we do not
        need a lock for this method. The producer can publish products until
        the queue reaches the maximum size.

        :type producer_id: String
        :param producer_id: producer id

        :type product: Product
        :param product: the Product that will be published in the Marketplace

        :returns True or False. If the caller receives False, it should wait and then try again.
        """
        logging.info('Entered publish - Producer Id: %s - Product: %s', producer_id, product)
        if len(self.producers[producer_id]) < self.producer_max_queue_size:
            self.producers[producer_id].append(product)
            logging.info('Left publish')
            return True
        logging.info('Left publish')
        return False

    def new_cart(self):
        """
        Creates a new cart for the consumer.
        Same as the register producer method.

        :returns an int representing the cart_id
        """
        logging.info('Entered new_cart')
        with self.new_cart_lock:
            current_id = self.ids["cart_ids"]
            self.carts[current_id] = []
            self.ids["cart_ids"] += 1
        logging.info('Left new_cart')
        return current_id

    def add_to_cart(self, cart_id, product):
        """
        Adds a product to the given cart. The method returns
        No need to add a lock here because retrieving keys from a dictionary is atomic.
        Removing a value from a list is atomic.
        Append is atomic.
        Retrieving a value from a list is atomic by de-referencing its index is atomic.
        It removes the product from the producer_ids list of available products and adds
        it into the cart by saving both the product and the id of the producer.
        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to add to cart

        :returns True or False. If the caller receives False, it should wait and then try again
        """
        logging.info('Entered add_to_cart - Cart Id: %d - Product: %s', cart_id, product)
        for producer_id in list(self.producers.keys()):
            if product in self.producers[producer_id]:
                self.producers[producer_id].remove(product)
                self.carts[cart_id].append([product, producer_id])
                logging.info('Left add_to_cart')
                return True
        logging.info('Left add_to_cart')
        return False

    def remove_from_cart(self, cart_id, product):
        """
        Removes a product from cart.
        Removing a value from a list is atomic.
        Append is atomic.
        In order to remove a product from a cart, we need the id of the producer
        given the way I represented the carts dictionary. It removes the
        product from the cart and appends it back to the producer's queue.
        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to remove from cart
        """
        logging.info('Entered remove_from_cart - Cart Id: %d - Product: %s', cart_id, product)
        producer_id = next(map(lambda prod: prod[1],
                               filter(lambda prod: prod[0] == product, self.carts[cart_id])))
        self.carts[cart_id].remove([product, producer_id])
        self.producers[producer_id].append(product)
        logging.info('Left remove_from_cart')

    def place_order(self, cart_id):
        """
        Return a list with all the products in the cart.
        We build a list of products for the current cart that called this function
        using list comprehension. We obtain the list of products from the cart and
        then empty the specific cart. Using the print_lock to print the results 
        in a thread safe manner. Printed the results here in order not to alter
        what this function should return. Alternative: return [products, self.lock_print],
        and then, using the lock, the products are printed in the consumer run method.
        :type cart_id: Int
        :param cart_id: id cart
        """
        logging.info('Entered place_order - Cart Id: %d', cart_id)
        products = [product[0] for product in self.carts[cart_id]]
        self.carts[cart_id] = []
        with self.print_lock:
            for product in products:
                print(f"{current_thread().name} bought {product}")
        logging.info('Left place_order')
        return products


class TestMarketplace(unittest.TestCase):
    """
    Class used to for unittesting.
    """
    def test_register_producer(self):
        """
        Method to test register_producer.
        """
        marketplace = Marketplace(2)

        producer_id1 = marketplace.register_producer()
        producer_id2 = marketplace.register_producer()
        producer_id3 = marketplace.register_producer()

        self.assertEqual(producer_id1, 0)
        self.assertEqual(producer_id2, 1)
        self.assertEqual(producer_id3, 2)

    def test_publish(self):
        """
        Method to test publish.
        """
        marketplace = Marketplace(2)

        producer_id = marketplace.register_producer()
        product_1 = Coffee("Arabica", 10, 5.1, "MEDIUM")
        product_2 = Tea("Linden", 9, "Herbal")
        product_3 = Coffee("Brasil", 15, 5.09, "MEDIUM")
        publish_product_1 = marketplace.publish(producer_id, product_1)
        publish_product_2 = marketplace.publish(producer_id, product_2)
        publish_product_3 = marketplace.publish(producer_id, product_3)

        self.assertEqual(publish_product_1, True)
        self.assertEqual(publish_product_2, True)
        self.assertEqual(publish_product_3, False)

    def test_new_cart(self):
        """
        Method to test new_cart.
        """
        marketplace = Marketplace(2)

        cart_id1 = marketplace.new_cart()
        cart_id2 = marketplace.new_cart()
        cart_id3 = marketplace.new_cart()

        self.assertEqual(cart_id1, 0)
        self.assertEqual(cart_id2, 1)
        self.assertEqual(cart_id3, 2)

    def test_add_to_cart(self):
        """
        Method to test add_to_cart.
        """
        marketplace = Marketplace(2)

        producer_id = marketplace.register_producer()
        product_1 = Coffee("Arabica", 10, 5.1, "MEDIUM")
        product_2 = Tea("Linden", 9, "Herbal")
        product_3 = Coffee("Brasil", 15, 5.09, "MEDIUM")

        marketplace.publish(producer_id, product_1)
        marketplace.publish(producer_id, product_2)

        cart_id = marketplace.new_cart()

        add_to_cart_product_1 = marketplace.add_to_cart(cart_id, product_1)
        add_to_cart_product_2 = marketplace.add_to_cart(cart_id, product_2)
        add_to_cart_product_3 = marketplace.add_to_cart(cart_id, product_3)

        self.assertEqual(add_to_cart_product_1, True)
        self.assertEqual(add_to_cart_product_2, True)
        self.assertEqual(add_to_cart_product_3, False)

        self.assertEqual(len(marketplace.carts[cart_id]), 2)
        self.assertEqual(len(marketplace.producers[producer_id]), 0)

    def test_remove_from_cart(self):
        """
        Method to test remove_from_cart.
        """
        marketplace = Marketplace(2)

        producer_id = marketplace.register_producer()

        product_1 = Coffee("Arabica", 10, 5.1, "MEDIUM")
        product_2 = Tea("Linden", 9, "Herbal")

        marketplace.publish(producer_id, product_1)
        marketplace.publish(producer_id, product_2)

        cart_id = marketplace.new_cart()

        marketplace.add_to_cart(cart_id, product_1)
        marketplace.add_to_cart(cart_id, product_2)

        marketplace.remove_from_cart(cart_id, product_2)

        self.assertEqual(len(marketplace.carts[cart_id]), 1)
        self.assertEqual(len(marketplace.producers[producer_id]), 1)

    def test_place_order(self):
        """
        Method to test place_order.
        """
        marketplace = Marketplace(2)

        producer_id = marketplace.register_producer()
        product_1 = Coffee("Arabica", 10, 5.1, "MEDIUM")
        product_2 = Tea("Linden", 9, "Herbal")

        marketplace.publish(producer_id, product_1)
        marketplace.publish(producer_id, product_2)

        cart_id = marketplace.new_cart()

        marketplace.add_to_cart(cart_id, product_1)
        marketplace.add_to_cart(cart_id, product_2)

        place_order_list = marketplace.place_order(cart_id)

        self.assertEqual(place_order_list, [product_1, product_2])

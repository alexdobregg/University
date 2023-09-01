"""
This module represents the Producer.

Computer Systems Architecture Course
Assignment 1
March 2023
"""

from threading import Thread
from time import sleep

class Producer(Thread):
    """
    Class that represents a producer.
    """

    def __init__(self, products, marketplace, republish_wait_time, **kwargs):
        """
        Constructor.

        @type products: List()
        @param products: a list of products that the producer will produce

        @type marketplace: Marketplace
        @param marketplace: a reference to the marketplace

        @type republish_wait_time: Time
        @param republish_wait_time: the number of seconds that a producer must
        wait until the marketplace becomes available

        @type kwargs:
        @param kwargs: other arguments that are passed to the Thread's __init__()
        """
        Thread.__init__(self, **kwargs)
        self.producer_id = marketplace.register_producer()
        self.products = products
        self.marketplace = marketplace
        self.republish_wait_time = republish_wait_time

    def run(self):
        """
        The iteration through all the products is done in an infinite loop.
        product[1] is the number of products
        product[0] is the id of the product
        product[2] is the sleeping time after a successful publish
        If the publish is successful, then it goes to the next product after some time.
        If the publish fails, then it waits for some time and then tries again.
        """
        while True:
            for product in self.products:
                for _ in range(product[1]):
                    while True:
                        if self.marketplace.publish(self.producer_id, product[0]):
                            sleep(product[2])
                            break
                        sleep(self.republish_wait_time)

"""
This module represents the Consumer.

Computer Systems Architecture Course
Assignment 1
March 2023
"""

from threading import Thread
from time import sleep

class Consumer(Thread):
    """
    Class that represents a consumer.
    """

    def __init__(self, carts, marketplace, retry_wait_time, **kwargs):
        """
        Constructor.

        :type carts: List
        :param carts: a list of add and remove operations

        :type marketplace: Marketplace
        :param marketplace: a reference to the marketplace

        :type retry_wait_time: Time
        :param retry_wait_time: the number of seconds that a producer must wait
        until the Marketplace becomes available

        :type kwargs:
        :param kwargs: other arguments that are passed to the Thread's __init__()
        """
        Thread.__init__(self, **kwargs)
        self.carts = carts
        self.marketplace = marketplace
        self.retry_wait_time = retry_wait_time

    def operation_loop(self, operation, cart_id, product, quantity, retry_wait_time):
        """
        Considering that we have a quantity for each product, that means the operation
        is executed that many times, regardless of its type. The add to cart returns either
        True or False, and the remove returns nothing. So if the operation was successful
        (not False), the next product operation is executed. If the operation fails, then
        the operation is executed again after some time.
        """
        for _ in range(quantity):
            while True:
                if operation(cart_id, product) is not False:
                    break
                sleep(retry_wait_time)

    def operation_handler(self, cart_id, operation):
        """
        Depending on the type of operation, the operation loop is called with either
        marketplace's add to cart or remove from cart methods.
        """
        if operation['type'] == 'add':
            self.operation_loop(self.marketplace.add_to_cart, cart_id, operation['product'],
                                operation['quantity'], self.retry_wait_time)
        else:
            self.operation_loop(self.marketplace.remove_from_cart, cart_id, operation['product'],
                                operation['quantity'], self.retry_wait_time)

    def run(self):
        """
        Creates a new cart by calling the marketplace new_cart function. 
        For every operation in each cart, the handler is called.
        After each operations was handled, the order is placed.
        """
        cart_id = self.marketplace.new_cart()
        for cart in self.carts:
            for operation in cart:
                self.operation_handler(cart_id, operation)
        self.marketplace.place_order(cart_id)

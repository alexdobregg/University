Name: Dobre Gigi-Alexandru
Group: 341C5

# Homework 1 - Marketplace - 2023

Solution
-
* The solution starts from the given classes and implements the methods required in each of them: Marketplace, Producer and Consumer.
* Neither Producer nor Consumer contain any synchronization elements as these elements are used in the Marketplace.
* The implementation is a good one, using only three locks because of the atomic methods present in python.
* The homework was very useful as I had the chance to have a deeper understanding on how threads are working.

Implementation
-
* All the requirements were implemented.
* I implemented a class for unittests and created tests for every method in Marketplace.
* I logged every entry and every exit from the methods. For the entry logs, when the method had parameters, they were logged as well.
* There are no extra functionalities. There are only two more methods defined in the Consumer class to avoid duplicated code.
* The only difficulty I had encountered was in the Consumer class because I've used the new_cart method inside the loop, as well as the place_order. This was causing a weird behaviour on the tenth test.
* The information about atomic methods was well received as I am sure it will be helpful for future python thread problems.


Resources
-
* https://superfastpython.com/thread-atomic-operations/
* https://ocw.cs.pub.ro/courses/asc/laboratoare/01
* https://ocw.cs.pub.ro/courses/asc/laboratoare/02
* https://ocw.cs.pub.ro/courses/asc/laboratoare/03
* https://docs.python.org/3/library/unittest.html#assert-methods
* https://docs.python.org/3/library/logging.html
* https://docs.python.org/3/library/logging.handlers.html#logging.handlers.RotatingFileHandler

Git
-
* https://github.com/alexdobregg/Tema1_ASC
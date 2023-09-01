# Project - Second Stage - Energy System

## About

Object-Oriented Programming Course - January 2021

<https://ocw.cs.pub.ro/courses/poo-ca-cd/teme/proiect/etapa2>

Student: Dobre Gigi-Alexandru 322CD

### Necessary libraries for implementation:
* Jackson Core 
* Jackson Databind 
* Jackson Annotations
* Json - 20140107
* Json - lib - 2.4 - jdk15
* org.json.simple - 0.3 - incubating

## Implementation
* src:
    * common:
        * Constants:
          This is a class containing all the constants I used in my
          implementation.
    * entities:
        * Consumer:
          This is a class whose object contains all the necessary information
          about a consumer.
        * Distributor:
          This is a class whose object contains all the necessary information
          about a distributor.
        * EnergyType:
          This is an ENUM containing types of energy created by energy
          producers.
        * EnergyTypeFactory:
          This is a class that is both Singleton and factory. Only one
          instance of this class can be created, and it returns the energy
          type for a producer.
        * Producer:
          This is a class whose object contains all the necessary information
          about a producer.
    * input:
        * Contract:
          This is a class whose object contains all the necessary information
          about a contract.
        * DistributorChanges:
          This is a class whose object contains all the necessary information
          about the infrastructure cost changes that occur every month.
        * Input:
          This is a class in whose object the information from the input file
          is stored.
        * InputLoader:
          This is a class that contains three methods called readData,
          readInitialData, readMonthlyUpdates. They are made for reading the
          data stored in the input file.
        * MonthlyStats:
          This is a class whose object contains all the necessary information
          about a producer's distributors in a specific month.
        * ProducerChanges:
          This is a class whose object contains all the necessary information
          about the energy per distributor changes that occur every month.
    * output:
        * OutputConsumer:
          This is a class containing the fields that are important about a
          consumer and must be written in the output file.
        * OutputDistributor:
          This is a class containing the fields that are important about a
          distributor and must be written in the output file.
        * OutputProducer:
          This is a class containing the fields that are important about a
          producer and must be written in the output file.
        * Writer:
          This is a class that creates an ObjectMapper and writes in the
          output file.
    * strategies:
        * EnergyChoiceFactory:
          This is a class that is both Singleton and factory. Only one
          instance of this class can be created, and it returns the type
          of strategy for a distributor.
        * EnergyChoiceStrategyType:
          This is an ENUM containing strategy types for distributors to
          choose their producers.
        * GreenSortingStrategy:
          This is a class whose object has a method to sort producers after
          energy type, price and quantity.
        * PriceSortingStrategy:
          This is a class whose object has a method to sort producers after
          price and quantity.
        * QuantitySortingStrategy:
          This is a class whose object has a method to sort producers after
          quantity.
        * SortingFactory:
          This is a class that is both Singleton and factory. Only one
          instance of this class can be created, and it returns the type of
          sorting strategy for distributors depending on their strategy type.
        * SortingStrategy:
          This is an interface that has a method called sort.
    * turns:
        * Turns:
          This is an interface that has a method called computeTurns. In
          addition to this, it also has two default methods that are
          common for both FirstTurn and OtherTurns. updateProducers updates
          the producers of a distributor and calculates the new production
          cost, while setDistributorBankrupt clears the contracts and removes
          the producers if a distributor goes bankrupt.
        * FirstTurn:
          This is a class that implements the Turns interface, defining
          its own implementation for the computeTurns, preparing
          everything for the next turns, like computing the prices
          of a distributor, creating contracts, consumers and
          distributors being paid and paying their costs and contract
          prices.
        * OtherTurns:
          This is a class that implements the Turns interface, defining
          its own implementation for the computeTurns, only this time,
          the actions repeat themselves for "number of turns" times. As
          well as the other class called FirstTurn, it calculates the
          prices, makes the changes that occur every month, creates
          new contracts, removes contracts that are ending or contracts
          that belong to bankrupt consumers. It also updates producers
          depending on the changes that occur for producers and the
          relations between producers and distributors.
        * TurnsFactory:
          This is a class that is both Singleton and factory. Only one
          instance of this class can be created, and it returns, based
          on what type it receives as an input, one new object of the
          classes that implements the Turns interface.
    * Main:
      This is a class used as an entry point for the program, where new
      objects are created, methods from these objects are called, where
      the fields of the output consumers, distributors and producers are set.
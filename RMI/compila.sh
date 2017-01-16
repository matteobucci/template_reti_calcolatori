#!/bin/bash

javac RMI_interfaceFile.java
javac RMI_Server.java

rmic -vcompat RMI_Server

javac RMI_Client.java

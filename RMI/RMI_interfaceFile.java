/*
 * Template RMI_interfaceFile.java
 * Matteo Bucci
 * 0000722984
 */

import java.rmi.RemoteException;

public interface RMI_interfaceFile extends java.rmi.Remote{

	String getEcho(String echo) throws RemoteException;
	String getInfo() throws RemoteException;	

}

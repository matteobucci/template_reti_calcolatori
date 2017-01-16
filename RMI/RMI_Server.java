/*
 * Template RMI_Server.java
 * Matteo Bucci
 * 0000722984
 */

import java.net.MalformedURLException;
import java.rmi.AlreadyBoundException;
import java.rmi.Naming;
import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.Calendar;


public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile{

	private static final long serialVersionUID = -3905607497715716286L;

	public RMI_Server() throws RemoteException{
		super();
	}
	
	public static void main(String[] args) {
		
		/*Variabili predefinite*/
		int registryPort = 1099;
		String registryHost = "localhost";
		String serviceName = "Server";

		/*Controllo argomenti*/
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: ServerImpl [registryPort]");
			System.exit(1);
		}
		if (args.length == 1) {
			try {
				registryPort = Integer.parseInt(args[0]);
			} catch (NumberFormatException e) {
				System.out.println("La porta inserita non è un intero.");
				System.exit(1);
			}
		}
		
		/*Avvio Security Manager*/
		if (System.getSecurityManager() == null){
			System.setSecurityManager(new RMISecurityManager()); 
		}
		
		/*Avvio server*/
		String serverName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		
		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.bind(serverName, serverRMI);
			System.out.println("Server RMI avviato con successo.");
		} catch (RemoteException e) {
			System.err.println("Impossibile avviare il server RMI:");
			e.printStackTrace();
		} catch (MalformedURLException e) {
			System.err.println("Impossibile avviare il server RMI, l'url inserito è ben formato?");
			e.printStackTrace();
		} catch (AlreadyBoundException e) {
			System.err.println("Impossibile avviare il server RMI, il server è già attivo?");
			e.printStackTrace();
		}
		
	}

	/*Metodi di prova*/
	@Override
	public String getEcho(String echo) throws RemoteException {
		return echo;
	}

	@Override
	public String getInfo() throws RemoteException {
		return "Server Java RMI. Creato da Matteo Bucci.\n";
	}

}

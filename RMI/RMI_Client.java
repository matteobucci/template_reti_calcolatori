/*
 * Template RMI_Client.java
 * Matteo Bucci
 * 0000722984
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;

public class RMI_Client {

	public static void main(String[] args) {
		
		int registryPort = 1099;
		String registryHost = null;
		String serviceName = "Server";
		RMI_interfaceFile serverRMI = null;
		String serverName;
		String richiesta = "Inserire qua una richiesta personalizzata: ";
	
		/*Controllo argomenti*/
		if (args.length != 1 && args.length != 2) {
			System.out.println("Sintassi: Client RegistryHost [registryPort]");
			System.exit(1);
		} else {
			try {
				registryHost = args[0];
				if (args.length == 2) {
					registryPort = Integer.parseInt(args[1]);
				}
			} catch (NumberFormatException e) {
				System.out.println("Inserire un numero intero come porta del registry");
				System.exit(1);
			}
		}
		
		/*Avvio Security Manager*/
		if (System.getSecurityManager() == null){
			System.setSecurityManager(new RMISecurityManager()); 
		}

		/*Connessione al server RMI*/
		serverName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		try {
			serverRMI = (RMI_interfaceFile) Naming.lookup(serverName);
		} catch (MalformedURLException | RemoteException | NotBoundException e) {
			System.err.println("Impossibile connettersi al server. Dettagli:");
			e.printStackTrace();
			System.exit(1);
		}

		/*Ciclo di richieste*/
		BufferedReader stdIn = null;
		String risposta = null;
		
		try {
			stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println(richiesta);
			while((risposta = stdIn.readLine()) != null){
				switch(risposta){
				case "a": //Metodo senza argomenti
					serverRMI.getInfo();
					break;
				case "b": //Metodo con argomento stringa
					System.out.println("Inserisci una stringa");
					risposta = stdIn.readLine();
					if(risposta != null){
						serverRMI.getEcho(risposta);
					}				
					break;
				default:
					System.out.println("Richiesta non riconosciuta");
				}
				System.out.println(richiesta);
			}
		} catch (IOException e) {
			System.err.println("Errore di IO. Dettagli: ");
			e.printStackTrace();
		}
		
		
	}
}

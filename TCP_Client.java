/*
 * Matteo Bucci
 * 0000722984
 */
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;


public class TCP_Client {

	public static final String CONSOLE_HINT = "Inserire l'input: ";
	
	public static void main(String[] args) {
		
		InetAddress serverAddr = null;
		int port = -1;
		Socket socket = null;
		
		/*Controllo argomenti*/
		if(args.length != 2){
			System.err.println("Numero argomenti errato. Utilizzo: java UDP_Unicast_Client serverIP serverPort");
			System.exit(1);
		}
		
		try{
			serverAddr = InetAddress.getByName(args[0]);
		}catch(UnknownHostException e){
			System.err.println("Impossibile determinare l'host: " + e.getMessage());
			e.printStackTrace();
		}
		
		try {
			port = Integer.parseInt(args[1]);
			if (port < 1024 || port > 65535) {
				System.err.println("Inserire un numero di porta nel range 1024-65535");
				System.exit(1);
			}
		} catch (NumberFormatException e) {
			System.err.println("Inserire un numero di porta valido");
			System.exit(1);
		}
		
		/*Creazione della socket*/
		try{
			socket = new Socket(serverAddr, port);
		}catch (IOException e) {
			System.err.println("Impossibile creare la socket");
			e.printStackTrace();
			System.exit(1);
		}
		
		/*Ciclo operativo del client*/
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		System.out.println(CONSOLE_HINT);

		try {
			DataInputStream dataInputStream = null;
			DataOutputStream dataOutputStream = null;
			String input;
			
			while ((input = stdIn.readLine()) != null) {
				/*Elaborazione risposta*/
				try {
					//TODO:ELABORAZIONE DATO DA CONSOLE
				} catch (Exception e) {
					//TODO:CATTURA EVENTUALI ECCEZIONI VERIFICATESI NELL'ELABORAZIONE DELL'INPUT
					System.out.println(CONSOLE_HINT);
					continue;
				}

				try {
					dataOutputStream = new DataOutputStream(socket.getOutputStream());
					
					//TODO:SCRIVERE I DATI RICHIESTI
					dataOutputStream.writeUTF(input);
					
					
				} catch (IOException e) {
					System.err.println("Impossibile inviare la richiesta: " + e.getMessage());
					e.printStackTrace();
					System.out.println(CONSOLE_HINT);
					continue;
				}

				
				try {
					dataInputStream = new DataInputStream(socket.getInputStream());
					
					System.out.println(dataInputStream.readUTF()); //Utilizzato come prova
					
				} catch (IOException e) {
					System.out.println("Problemi nella lettura della risposta: " + e.getMessage());
					e.printStackTrace();
					System.out.println(CONSOLE_HINT);
					continue;
				}
			
				System.out.println(CONSOLE_HINT);
			} 
		}
		catch (Exception e) {
			System.err.println("Eccezione non prevista: " + e.getMessage());
			e.printStackTrace();
		}
		
		try {
			socket.close();
		} catch (IOException e) {
			System.err.println("Impossibile chiudere la socket");
			e.printStackTrace();
		}
		System.out.println("Effettuata la chiusura del client.");
	}
}





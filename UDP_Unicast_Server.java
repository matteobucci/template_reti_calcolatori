/*
 * Matteo Bucci
 * 0000722984
 */

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class UDP_Unicast_Server {

	private static final int DEFAULT_PORT = 10000;
	private static final int PACKET_SIZE = 256;

	public static void main(String[] args) {
	
		DatagramSocket socket = null;
		DatagramPacket packet = null;
		byte[] buf = new byte[PACKET_SIZE];
		byte[] data;
		int port = -1;

		/*Controllo argomenti*/
		if ((args.length == 0)) {
			port = DEFAULT_PORT;
		} else if (args.length == 1) {
			try {
				port = Integer.parseInt(args[0]);
				if (port < 1024 || port > 65535) {
					System.err.println("Inserire un numero di porta nel range 1024-65535");
					System.exit(1);
				}
			} catch (NumberFormatException e) {
				System.err.println("Inserire un numero di porta valido");
				System.exit(1);
			}
		} else {
			System.err.println("Argomenti inseriti non validi. Uso: java UDP_Unicast_Server [port]");
			System.exit(1);
		}
		
		/*Creazione socket e packet*/
		try {
			socket = new DatagramSocket(port);
			packet = new DatagramPacket(buf, buf.length);
			System.out.println("Socket creata con successo");
		}
		catch (SocketException e) {
			System.err.println("Problemi nella creazione della socket");
			e.printStackTrace();
			System.exit(1);
		}
		
		/*Ciclo operativo del server*/
		try {
			
			ByteArrayInputStream byteInputStream = null;
			DataInputStream dataInputStream = null;
			ByteArrayOutputStream byteOutputStream = null;
			DataOutputStream dataOutputStream = null;
			
			System.out.println("Server in ascolto sulla porta " + port);
			
			while (true) {				
				
				/*Ricezione pacchetto*/
				try {
					packet.setData(buf);
					System.out.println("SetData fatta. Dimensione: " + packet.getLength());
					socket.receive(packet);
				}
				catch (IOException e) {
					System.err.println("Problemi nella ricezione del datagramma: " + e.getMessage());
					e.printStackTrace();
					continue;
				}
				
				System.out.println("Datagram ricevuto.");
				System.out.println("Informazioni sull'host:");
				System.out.println("\t IP: " + packet.getAddress().getCanonicalHostName());
				System.out.println("\t Port: " + packet.getPort());

				/*Elaborazione dati*/
				try{
					byteInputStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
					dataInputStream = new DataInputStream(byteInputStream);
					
					//TODO:ELABORARE I DATI RICHIESTI
				
				}catch(Exception e){ 
					//TODO: SPECIALIZZARE L'ECCEZIONE
					e.printStackTrace();
					continue;
				}
				
				//TODO:EFFETTUARE LA LETTURA DEI DATI RICHIESTI
					
				
				/*Invio risposta*/
				try{
					byteOutputStream = new ByteArrayOutputStream();
					dataOutputStream = new DataOutputStream(byteOutputStream);
					
					//TODO:SCRIVERE I DATI RICHIESTI
					dataOutputStream.writeUTF(dataInputStream.readUTF());
					
					data = byteOutputStream.toByteArray();
					packet.setData(data, 0, data.length);
					socket.send(packet);
				}
				catch (IOException e) {
					System.err.println("Impossibile inviare la risposta: " + e.getMessage());
					e.printStackTrace();
					continue;
				}	
				
			}
		}
		catch (Exception e) {
			System.err.println("Eccezione non prevista: " + e.getMessage());
			e.printStackTrace();
		}
		socket.close();
		System.out.println("Effettuata la chiusura del server.");
	}
}

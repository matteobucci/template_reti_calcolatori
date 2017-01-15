/*
 * Matteo Bucci
 * 0000722984
 */

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;


public class UDP_Unicast_Client {
	
	private static final int SOCKET_TIMEOUT = 30000;
	private static final int PACKET_SIZE = 2048;
	public static final String CONSOLE_HINT = "Inserire l'input: ";

	public static void main(String[] args) {
		
		InetAddress serverAddr = null;
		int port = -1;
		DatagramSocket socket = null;
		DatagramPacket packet = null;
		byte[] buf = new byte[PACKET_SIZE];
		
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
		
		/*Creazione socket e packet*/
		try {
			socket = new DatagramSocket();
			socket.setSoTimeout(SOCKET_TIMEOUT);
			packet = new DatagramPacket(buf, buf.length, serverAddr, port);
			System.out.println("Socket creata con successo");
		}
		catch (SocketException e) {
			System.err.println("Problemi nella creazione della socket");
			e.printStackTrace();
			System.exit(1);
		}
		
		/*Ciclo operativo del client*/
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		System.out.println(CONSOLE_HINT);

		try {
			
			ByteArrayInputStream byteInputStream = null;
			DataInputStream dataInputStream = null;
			ByteArrayOutputStream byteOutputStream = null;
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

				/*Riempimento e invio pacchetto*/
				try {
					byteOutputStream = new ByteArrayOutputStream();
					dataOutputStream = new DataOutputStream(byteOutputStream);
					
					//TODO:SCRIVERE I DATI RICHIESTI
					dataOutputStream.writeUTF(input);
					
					buf = byteOutputStream.toByteArray();
					packet.setData(buf);
					socket.send(packet);
					System.out.println("Pacchetto inviato a " + serverAddr.getHostName() + ":" + port);
					System.out.println("Dimensione: " + packet.getLength());
					System.out.println("In attesa di risposta...");
				} catch (IOException e) {
					System.err.println("Impossibile inviare la richiesta: " + e.getMessage());
					e.printStackTrace();
					System.out.println(CONSOLE_HINT);
					continue;
				}

				/*Ricezione pacchetto ed elaborazione dati*/
				try {
					packet.setData(buf);
					socket.receive(packet);
				} catch (IOException e) {
					System.err.println("Impossibile ricevere la risposta: " + e.getMessage());
					e.printStackTrace();
					System.out.println(CONSOLE_HINT);
					continue;
				}
				try {
					byteInputStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
					dataInputStream = new DataInputStream(byteInputStream);
					
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
		socket.close();
		System.out.println("Effettuata la chiusura del client.");
	}
}

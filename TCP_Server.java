/*
 * Matteo Bucci
 * 0000722984
 */
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;

class ServerThread extends Thread {

	private Socket clientSocket = null;

	public ServerThread(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	public void run() {
		DataInputStream dataInputStream;
		DataOutputStream dataOutputStream;

		System.out.println("Attivazione figlio "+ Thread.currentThread().getName());
		System.out.println("Informazioni sulla connessione: ");
		System.out.println("\tIndirizzo IP: " + clientSocket.getInetAddress().getHostName());
		System.out.println("\tPorta: " + clientSocket.getPort());
		
		try {
			try {
				dataInputStream = new DataInputStream(clientSocket.getInputStream());
				dataOutputStream = new DataOutputStream(clientSocket.getOutputStream());
			} catch (IOException e) {
				System.err.println("Impossibile creare gli stream associati alla scocket");
				e.printStackTrace();
				return;
			}

			try {
				String stringaRicevuta;
				
				while (true) {
					while ((stringaRicevuta = dataInputStream.readUTF()) != null) {
							System.out.println("Ricevuta stringa: " + stringaRicevuta);
							dataOutputStream.writeUTF(stringaRicevuta);
						}
				} 
			} catch (EOFException e) {
				System.out.println("Il client ha terminato il collegamento. Chiudo la socket e termino il thread");
				clientSocket.close();			
			} catch (SocketTimeoutException e) {
				System.err.println("Timeout scattato. Chiudo la socket e termino il thread");
				e.printStackTrace();
				clientSocket.close();
			} catch (Exception e) {
				System.err.println("Eccezione non prevista. Chiudo la socket e termino il thread");
				e.printStackTrace();
				clientSocket.close();
			}
		}catch (Exception e) {
			System.err.println("Eccezione non prevista. Chiudo la socket e termino il thread");
			e.printStackTrace();
		}
		System.out.println("Terminazione figlio: " + Thread.currentThread().getName());
	}

}


public class TCP_Server {

	private static final int DEFAULT_PORT = 10000;
	
	public static void main(String[] args) {
		
		int port = -1;
		ServerSocket serverSocket = null;

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
				e.printStackTrace();
				System.exit(1);
			}
		} else {
			System.err.println("Argomenti inseriti non validi. Uso: java TCP_Server [port]");
			System.exit(1);
		}
		
		/*Creazione della socket server*/
		try {
			serverSocket = new ServerSocket(port);
		} catch (IOException e) {
			System.err.println("Impossibile creare la server socket");
			e.printStackTrace();
			System.exit(1);
		}
		
		try {
			System.out.println("Server avviato ("+ serverSocket.getInetAddress().getHostName() + ":" + serverSocket.getLocalPort() + "). In ascolto di connessioni...");
			while(true){
				Socket socket = serverSocket.accept();
				new ServerThread(socket).start();
				
			}
		} catch (IOException e) {
			System.err.println("Impossibile effettuare l'accept");
			e.printStackTrace();
			System.exit(1);
		}
		
		try {
			System.out.println("Chiusura server socket in corso.");
			serverSocket.close();
		} catch (IOException e) {
			System.err.println("Impossibiel chiudere la server socket");
			e.printStackTrace();
		}
		
		System.out.println("Terminazione server");
		
	}

}

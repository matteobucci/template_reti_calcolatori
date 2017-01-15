import java.util.Random;

public class InizializzaC {

	public static int PERCENTUALE_OCCUPATI = 30;
	public static int MAX_VALORE_INT = 100;
	public static String PAROLE[] = {"Casa","Albero","Signore","Signora","Capra","Cigno","Villa","Elemento","Festa","Pulce"};
	public static String AGGETTIVI[] = {"Brutto","Bello","Alto","Basso","Medio","Normale","Straordinario","Carino","Perfetto","Difettoso"};
	
	public static void main(String[] args) {
		
		int numeroElementi = 0;
		Random random = new Random();
		
		if(args.length <= 2){
			System.err.println("Uso: numeroElementi nomeStruttura {nomeElemento tipoElemento(int/string)}");
			System.exit(1);
		}
		try{
			numeroElementi = Integer.parseInt(args[0]);
		}catch(NumberFormatException e){
			System.err.println("Inserire un numero di elementi valido");
			System.exit(1);
		}
		
		if(numeroElementi < 1 || args.length%2 != 0){
			System.err.println("Inserire un numero di elementi valido e fornire i dati necessari per formare gli elementi (nome e tipo)");
			System.exit(1);
		}
		
		System.out.println("void inizializza(){");
		System.out.println("\tint i;\n");
		System.out.println("\tfor (i = 0; i < "+ numeroElementi + "; i++){\n");
		
		for(int i=2; i<args.length; i+=2){
			
			if(args[i+1].equals("int")){
				System.out.println("\t\t" + args[1] + "[i]." + args[i] +" = -1;");
				System.out.println("");
			}else if(args[i+1].equals("stringa")){
				System.out.println("\t\t" + args[1] + "[i]." + args[i] +" = (char*) malloc(2);");
				System.out.println("\t\t" + "strcpy(" + args[1] + "[i]." + args[i] + ", \"L\");");
				System.out.println("");
			}else{
				System.out.println("\t\t//Tipo '" + args[i+1] + "' non riconosciuto." );
			}
			
		}
		
		System.out.println("\t}");
		
		int numeroOccupati = numeroElementi * PERCENTUALE_OCCUPATI / 100;
		
		/*Es:
		*	100 elementi
		*	30 occupati
		*	numeroOccupati = 30
		*	numeroOccupati/numeroOccupati = 3.33
		*	Dovrebbe fare 33 interazioni circa (a meno delle approsimazioni)
		*/
		
		for(int j=0; j < numeroElementi; j+= numeroElementi/numeroOccupati){
			
			System.out.println("\t//Elemento numero" + (j+1));
			
			for(int i=2; i<args.length; i+=2){
								
				if(args[i+1].equals("int")){
					System.out.println("\t" + args[1] + "["+j+"]." + args[i] +" = "+ random.nextInt(MAX_VALORE_INT) + ";");
				}else if(args[i+1].equals("stringa")){
					
					int numeroParola = random.nextInt(PAROLE.length);
					int numeroAggettivo = random.nextInt(AGGETTIVI.length);
					String elemento = PAROLE[numeroParola] + AGGETTIVI[numeroAggettivo];
					
					System.out.println("\tfree(" + args[1] + "["+j+"]." + args[i] +");");
					System.out.println("\t" + args[1] + "["+j+"]." + args[i] +" = (char*) malloc(strlen(\""+ elemento +"\") + 1);");
					System.out.println("\tstrcpy(" + args[1] + "["+j+"]." + args[i] +", \""+ elemento +"\");");
				}else{
				
				}
								
			}
			
			System.out.println("");
		}
		
		
		
		
		System.out.println("\t}");
		System.out.println("\tinizializzato = 1;");
		System.out.println("}");
		
		

	}

}

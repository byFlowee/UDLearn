import java.util.ArrayList;

public class Main 
{
	
	public final static String PHRASE = "To Be Or Not To Be That Is The Question";
	
	public static Integer NonGeneticAlgorithm(Integer max)
	{
		Phrase p = new Phrase(Main.PHRASE);
		Integer res = 0;
		
		if (max < 0)
		{
			while (true)
			{
				if (Util.getRandomPhrase(Main.PHRASE.length()).equals(Main.PHRASE))
				{
					return res;
				}
				
				res++;
			}
		}
		else
		{
			for (int i = 0; i < max; i++)
			{
				String cadena = Util.getRandomPhrase(Main.PHRASE.length());
				
				System.out.println("Generacion " + i + ": " + cadena);
				
				if (cadena.equals(Main.PHRASE))
				{
					return res;
				}
				
				res++;
			}
			
			return 0;
		}
	}
	
	public static Integer GeneticAlgorithm(Integer max, Double mutationRate, Integer population)
	{
		PhraseGeneticAlgorithm p = new PhraseGeneticAlgorithm(Main.PHRASE, population);
		
		ADN.setMutacion(mutationRate);
		
		p.crearPoblacion();
		
		for (int i = 0; i < max; i++)
		{
			p.calcularFitness();
			
			String best = p.obtenerMejorFitness();
			
			System.out.println("Generacion " + p.getGeneracion() + ": " + best);
			
			if (best.equals(Main.PHRASE))
			{
				break;
			}
			
			p.reemplazarPoblacion();
		}
		
		return p.getGeneracion();
	}

	public static void main(String[] args)
	{
		Main.GeneticAlgorithm(100000, 0.001, 550);
		//Main.NonGeneticAlgorithm(10000);
	}
	
}

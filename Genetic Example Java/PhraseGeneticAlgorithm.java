import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

public class PhraseGeneticAlgorithm 
{
	
	private static Integer POBLACION = 200;
	
	private ArrayList<ADN> poblacion;
	
	private ArrayList<Integer> fitness;
	
	private String objetivo;
	
	private Integer generacion;

	public PhraseGeneticAlgorithm(String objetivo)
	{
		this.poblacion = new ArrayList<>();
		this.fitness = new ArrayList<>();
		this.generacion = 0;
		this.objetivo = objetivo; 
	}
	
	public PhraseGeneticAlgorithm(String objetivo, Integer population)
	{
		this.poblacion = new ArrayList<>();
		this.fitness = new ArrayList<>();
		this.generacion = 0;
		this.objetivo = objetivo;
		PhraseGeneticAlgorithm.POBLACION = population;
	}
	
	public void crearPoblacion()
	{
		if (this.poblacion.size() == 0)
		{
			for (int i = 0; i < PhraseGeneticAlgorithm.POBLACION; i++)
			{
				this.poblacion.add(i, new ADN(Util.getRandomPhrase(this.objetivo.length())));
			}
		}
		else
		{
			for (int i = 0; i < PhraseGeneticAlgorithm.POBLACION; i++)
			{
				this.poblacion.set(i, new ADN(Util.getRandomPhrase(this.objetivo.length())));
			}
		}
	}
	
	private Integer fitness(String s)
	{
		Integer res = 0;
		
		for (int i = 0; i < this.objetivo.length(); i++)
		{
			if (this.objetivo.charAt(i) == s.charAt(i))
			{
				res++;
			}
		}
		
		return res;
	}
	
	public void calcularFitness()
	{
		if (this.fitness.size() == 0)
		{
			for (int i = 0; i < this.poblacion.size(); i++)
			{
				this.fitness.add(i, this.fitness(this.poblacion.get(i).getPhrase()));
			}			
		}
		else
		{
			for (int i = 0; i < this.poblacion.size(); i++)
			{
				this.fitness.set(i, this.fitness(this.poblacion.get(i).getPhrase()));
			}
		}
	}
	
	private Integer getRandom()
	{
		Integer sum = 0;
		
		for (Integer i : this.fitness)
		{
			sum += i;
		}
		
		Integer index = 0;
		Double random = ThreadLocalRandom.current().nextDouble(0.0, 1.0);
		
		for (int i = 0; i < this.fitness.size(); i++)
		{
			random -= (double)this.fitness.get(i) / (double)sum;
			
			if (random < 0.0)
			{
				index = i;
				break;
			}
		}
		
		return index;
	}
	
	public ADN hacerCrossover()
	{
		ADN nuevo, padre, madre;
		
		padre = this.poblacion.get(this.getRandom());
		madre = this.poblacion.get(this.getRandom());
		nuevo = padre.crossover(madre);
		
		return nuevo;
	}
	
	public String obtenerMejorFitness()
	{
		Integer best = -1;
		Integer index = 0;
		
		for (int i = 0; i < this.fitness.size(); i++)
		{
			if (this.fitness.get(i) > best)
			{
				best = this.fitness.get(i);
				index = i;
			}
		}
		
		return this.poblacion.get(index).getPhrase();
	}
	
	public void hacerMutacion(ADN a)
	{
		a.mutacion();
	}
	
	public void reemplazarPoblacion()
	{
		ArrayList<ADN> nuevaPoblacion = new ArrayList<>();
		
		for (int i = 0; i < this.POBLACION; i++)
		{
			ADN hijo = this.hacerCrossover();
			
			this.hacerMutacion(hijo);
			
			nuevaPoblacion.add(hijo);
		}
		
		this.poblacion = nuevaPoblacion;
		this.generacion++;
	}
	
	public Integer getGeneracion()
	{
		return this.generacion;
	}
	
}

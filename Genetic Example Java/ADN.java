import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

public class ADN 
{
	
	private ArrayList<Character> genes;
	
	private static Double mutacion = 0.01;
	
	public ADN(String s)
	{
		this.genes = new ArrayList<>();
		
		for (Character c : s.toCharArray())
		{
			this.genes.add(c);
		}
	}
	
	public static void setMutacion(Double mutacion)
	{
		ADN.mutacion = mutacion;
	}
	
	/*
	public ADN crossover(ADN a)
	{
		ADN res = new ADN("");
		Integer half = this.genes.size();
		
		for (int i = 0; i < half; i++)
		{
			res.genes.add(this.genes.get(i));
		}
		
		for (int i = half; i < this.genes.size(); i++)
		{
			res.genes.add(a.genes.get(i));
		}
		
		return res;
	}
	*/
	
	public ADN crossover(ADN a)
	{
		ADN res = new ADN("");
		
		for (int i = 0; i < this.genes.size(); i++)
		{
			if (i % 2 == 0)
			{
				res.genes.add(this.genes.get(i));
			}
			else
			{
				res.genes.add(a.genes.get(i));
			}
		}
		
		return res;
	}
	
	public void mutacion()
	{
		for (int i = 0; i < this.genes.size(); i++)
		{
			Double random = ThreadLocalRandom.current().nextDouble(0.0, 1.0);
			
			if (random <= ADN.mutacion)
			{
				this.genes.set(i, Util.getRandomCharacter());
			}
		}
	}
	
	public String getPhrase()
	{
		String s = "";
		
		for (Character c : this.genes)
		{
			s += c;
		}
		
		return s;
	}
	
}

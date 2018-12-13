import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

public class Util 
{
	
	public static Character getRandomCharacter()
	{
		char[] abcd = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
					   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
					   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
					   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
					   ' '};
		
		Character res = abcd[ThreadLocalRandom.current().nextInt(0, abcd.length)];

		return res;
	}

	public static String getRandomPhrase(Integer length)
	{
		String res = "";
		
		for (int i = 0; i < length; i++)
		{
			res += Util.getRandomCharacter();
		}
		
		return res;
	}
	
	public static int test(ArrayList<Integer> a)
	{
		int sum = 0;

		for (Integer i : a)
		{
			sum += i;
		}
		
		Double random = ThreadLocalRandom.current().nextDouble(0.0, 1.0);
		
		for (int i = 0; i < a.size(); i++)
		{
			random -= (double)a.get(i) / (double)sum;
			
			if (random < 0.0)
			{
				return i;
			}
		}
		
		return -1;
	}
	
}

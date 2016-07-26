// runme.java

public class runme {
	static {
		System.loadLibrary("stringex");
	}

	public static void main(String argv[]) {
		StringEx obj = new StringEx();
		obj.addName("laxman");
		obj.addName("janaki");
		obj.addName("ram123");
		obj.printAll();
		String x = obj.getNameAt(1);
		System.out.println(x);
		System.out.println(obj.getSize());
	}
}

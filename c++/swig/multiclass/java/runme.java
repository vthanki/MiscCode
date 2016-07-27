// runme.java

public class runme {
	static {
		System.loadLibrary("multiclass");
	}

	public static void main(String argv[]) {
		Polygon p = new Polygon();
		p.addSide(new Side(10));
		p.addSide(new Side(30));
		p.addSide(new Side(10));
		p.addSide(new Side(10));
		System.out.println(p.getPerimeter());
	}
}

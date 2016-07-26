// runme.java

public class runme {
  static {
    System.loadLibrary("cpp_calc");
  }

  public static void main(String argv[]) {
    System.out.println(cpp_calc.getFinalResult(500,2,'/'));
  }
}

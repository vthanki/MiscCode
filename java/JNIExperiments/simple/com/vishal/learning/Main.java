package com.vishal.learning;

public class Main {
	static {
		System.loadLibrary("hello"); // Load native library at runtime
	}

	// Declare a native method sayHello() that receives nothing and returns void
	private static native void sayHello();
	private static native Polygon createPolygon();
	private static native void addSideToPolygon(Polygon p, String dummy);

	// Test Driver
	public static void main(String[] args) {
		Main.sayHello();  // invoke the native method
		Polygon p = Main.createPolygon();
		Main.addSideToPolygon(p,"Dummy");
		System.out.println(p.getPerimeter());
	}
}

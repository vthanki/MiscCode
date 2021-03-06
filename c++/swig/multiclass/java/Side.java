/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.7
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class Side {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected Side(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Side obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        MultiClass_SwigJNI.delete_Side(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public Side() {
    this(MultiClass_SwigJNI.new_Side__SWIG_0(), true);
  }

  public Side(int l) {
    this(MultiClass_SwigJNI.new_Side__SWIG_1(l), true);
  }

  public int getSideLength() {
    return MultiClass_SwigJNI.Side_getSideLength(swigCPtr, this);
  }

}

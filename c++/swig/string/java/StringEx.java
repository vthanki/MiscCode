/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.7
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class StringEx {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected StringEx(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(StringEx obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        StringEx_SwigJNI.delete_StringEx(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void addName(String n) {
    StringEx_SwigJNI.StringEx_addName(swigCPtr, this, n);
  }

  public String getNameAt(int i) {
    return StringEx_SwigJNI.StringEx_getNameAt(swigCPtr, this, i);
  }

  public void printAll() {
    StringEx_SwigJNI.StringEx_printAll(swigCPtr, this);
  }

  public int getSize() {
    return StringEx_SwigJNI.StringEx_getSize(swigCPtr, this);
  }

  public StringEx() {
    this(StringEx_SwigJNI.new_StringEx(), true);
  }

}

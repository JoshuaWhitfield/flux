package functional.iterators.states;

public class IteratorState {
    private boolean end = false;
    private boolean cont = false;
    private boolean back = false;

    public boolean isEnd() { return end; }
    public void setEnd(boolean val) { end = val; }
    
    public boolean isCont() { return cont; }
    public void setCont(boolean val) { cont = val; }

    public boolean isBack() { return back; }
    public void setBack(boolean val) { back = val; }
}

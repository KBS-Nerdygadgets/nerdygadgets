package TSP2;

/*
    Class voor het aanmaken van de coördinaten van de punten.
    Daarnaast wordt er gekeken of dit punt wel of niet bezocht is.
*/
public class Point {
    int x;
    int y;
    boolean visited;
    
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
        this.visited = false;
    }

    public int getX(){
        return x;
    }

    public int getY(){
        return y;
    }
}

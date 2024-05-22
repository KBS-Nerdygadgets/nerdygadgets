package TSP;

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
        // Getter for x
        public int getX() {
            return x;
        }
    
        // Getter for y
        public int getY() {
            return y;
        }
}

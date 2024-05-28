package TSP2;

import java.util.ArrayList;
import java.util.List;

public class Main {
    Point nul = new Point(1, 5);
    public static void main(String[] args) {

        
        
        // Voorbeeld van de mogelijke punten in het magazijn
        Point[] pointsArray = {
//                new Point(0, 0), new Point(0, 1), new Point(0, 2), new Point(0, 3), new Point(0, 4),
//                new Point(1, 0), new Point(1, 1), new Point(1, 2), new Point(1, 3), new Point(1, 4),
//                new Point(2, 0), new Point(2, 1), new Point(2, 2), new Point(2, 3), new Point(2, 4),
//                new Point(3, 0), new Point(3, 1), new Point(3, 2), new Point(3, 3), new Point(3, 4),
//                new Point(4, 0), new Point(4, 1), new Point(4, 2), new Point(4, 3), new Point(4, 4)
                new Point(1, 6), new Point(3, 2), new Point(1, 2), new Point(0, 3), new Point(5, 1),
                new Point(6, 6), new Point(4, 3), new Point(3, 5), new Point(2, 4), new Point(2, 3),
        };

        // Hier wordt de nieuZwe array 'points' aangemaakt.
        // Aan de array 'points' worden de punten van 'pointsArray' toegevoegd
        List<Point> points = new ArrayList<>();
        for (Point p : pointsArray){
            points.add(p);
            }

        // Het startpunt van de robot
        Point start = points.remove(0);

        // Hier wordt het Nearest Neighbor-algoritme toegepast
        List<Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

        // Hier wordt het gevonden pad uitgeschreven
        System.out.println("Gevonden pad:");
        for (Point p : path) {
            System.out.println("Punt: (" + p.x + ", " + p.y + ")");
        }
    }
}

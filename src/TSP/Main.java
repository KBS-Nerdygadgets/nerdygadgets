package TSP;

import java.util.ArrayList;
import java.util.List;
import Database.Database;

public class Main {
    public static void main(String[] args) {
        // Punten halen van de database
        List<Point> points = Database.fetchPointsFromDatabase();

        // Als er geen punten zijn, stoppen
        if (points.isEmpty()) {
            System.out.println("No points found in the database.");
            return;
        }

        // Het startpunt van de robot
        Point start = points.remove(0);

        // De Nearest Neighbour Algorithm toepassen
        List<Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

        // Het gevonden pad uitprinten
        System.out.println("Gevonden pad:");
        for (Point p : path) {
            System.out.println("Punt: (" + p.x + ", " + p.y + ")");
        }
    }
}

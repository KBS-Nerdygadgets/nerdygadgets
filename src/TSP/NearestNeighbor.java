package TSP;

import java.util.ArrayList;
import java.util.List;
public class NearestNeighbor {

    // Functie die de afstand berekent tussen twee punten
    static double distance(Point p1, Point p2) {
        return Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
    }

    // Functie die vanaf het huidige punt het dichtstbijzijnde onbezochte punt vindt
    static Point nearestNeighbor(Point current, List<Point> points) {
        Point nearest = null;
        double minDistance = Double.MAX_VALUE;
        for (Point point : points) {
            if (!point.visited) {
                double distance = distance(current, point);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = point;
                }
            }
        }
        return nearest;
    }

    // Het Nearest Neighbor-algoritme
    static List<Point> nearestNeighborAlgorithm(Point start, List<Point> points) {
        List<Point> path = new ArrayList<>();
        Point current = start;          // Het startpunt wordt gezet als het huidige punt
        current.visited = true;
        path.add(current);

        // Hier wordt steeds het dichtstbijzijnde onbezochte punt als huidige punt gezet
        while (true) {
            Point nearest = nearestNeighbor(current, points);
            if (nearest == null) // Alle punten zijn bezocht
                break;
            nearest.visited = true;
            path.add(nearest);
            current = nearest;
        }

        return path;
    }
}

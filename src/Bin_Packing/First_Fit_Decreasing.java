package Bin_Packing;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class First_Fit_Decreasing {
    public static ArrayList<ArrayList<Integer>> firstFitDecreasing(ArrayList<Integer> stockItemIDs){
        int binCapaciteit = 10;
        ArrayList<Product> alleProducten = new ArrayList<>();
        for (Integer stockItemID : stockItemIDs) {
            alleProducten.add(new Product(stockItemID));
        }
        Collections.sort(alleProducten, new Comparator<Product>() {
            public int compare(Product p1, Product p2) {
                return Integer.compare(p2.gewicht, p1.gewicht);
            }
        });

        ArrayList<Bin> bins = new ArrayList<>();
        bins.add(new Bin(binCapaciteit));

        for(Product eenProduct : alleProducten){

            boolean waardeToegepast = false;

            if(eenProduct.gewicht > binCapaciteit){
                System.out.println("Getal " + eenProduct.gewicht + " wordt overgeslagen omdat deze groter is dan de capaciteit.");
                continue;
            }

            for(Bin bin : bins){
                if(bin.voegItemToe(eenProduct)){
                    waardeToegepast = true;
                    break;
                }
            }

            if(!waardeToegepast){
                Bin nieuweBin = new Bin(binCapaciteit);
                nieuweBin.voegItemToe(eenProduct);
                bins.add(nieuweBin);
            }
        }
        ArrayList<ArrayList<Integer>> BPP = new ArrayList<>();
        for (Bin bin : bins) {
            ArrayList<Integer> lijstInhoud = new ArrayList<>();
            for (Product product : bin.producten) {
                lijstInhoud.add(product.stockItemID);
            }
            BPP.add(lijstInhoud);
        }
        return BPP;
    }

    public static void printBins(ArrayList<Bin> bins){
        for(int i = 0; i < bins.size(); i++){
            System.out.println("Bin " + (i + 1) + ": Capaciteit = " + bins.get(i).capaciteit + ", Ruimte over = " + bins.get(i).ruimte + " " + bins.get(i).producten);
        }
    }
}



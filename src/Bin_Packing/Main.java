package Bin_Packing;


import java.util.ArrayList;

public class Main {
    public static void main(String[] args){
        ArrayList<Integer> productID = new ArrayList<>();

        productID.add(67);
        productID.add(50);
        productID.add(10);
        productID.add(114);
        productID.add(206);
        productID.add(130);
        productID.add(50);
        productID.add(128);
        productID.add(121);

//        ArrayList<Bin> bins = First_Fit_Decreasing.firstFitDecreasing(productID);
//        First_Fit_Decreasing.printBins(bins);

        System.out.println(First_Fit_Decreasing.firstFitDecreasing(productID));
    }
}

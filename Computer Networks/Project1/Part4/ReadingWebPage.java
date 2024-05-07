package network;
import org.jsoup.Connection;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class ReadingWebPage {
    public static void main(String[] args) throws IOException {
        Scanner getIt = new Scanner(System.in);
        System.out.println("Please enter the link of the page");
        String page = getIt.next();
        System.out.println("Please enter the word you need to search for");
        String wordForSearch = getIt.next();
        //Connecting to the web page
        Connection connect = Jsoup.connect(page);
        //executing the get request
        Document d = connect.get(); //connection get request
        //Retrieving the contents (body) of the web page
        String result = d.body().text();
        String [] words = result.split(" ");
        long count = 0;
        for (int i=0;i<words.length; i++){
            if (wordForSearch.equalsIgnoreCase(words[i])){
                count++;
            }
        }
//        List<String> words = new ArrayList<>(Arrays.asList(result.split(" ")));
//        long count = words.stream().filter(word -> word.equalsIgnoreCase(wordForSearch)).count();
        System.out.println("the count of " + wordForSearch + " word is " + count);
    }
}


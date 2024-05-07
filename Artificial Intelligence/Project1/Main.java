package com.example.ai_project;
import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Scene;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javax.swing.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.*;

public  class Main extends Application implements Initializable {
    public static Stage stage;
    @FXML
    private ScrollPane Scroll_pane;
    @FXML
    private TextField textFieldPathFile;
    TablesController tablesView;

    public static ArrayList<Course> Courses = new ArrayList<>();
    public static Set<String> doctorsNameSet =new HashSet<>();
    public static Set<String> coursesNameSet =new HashSet<>();
    public static Set<String> coursesCode =new HashSet<>();

    public Main() {
    }

    @Override
    public void start(Stage primaryStage) throws IOException {

        FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource("View.fxml"));
        stage= primaryStage;
        Scene scene = new Scene(fxmlLoader.load(), 1633, 950);
        primaryStage.setTitle(" Department courses distribution");
        primaryStage.setScene(scene);
        primaryStage.show();

    }

    @FXML
    void btnBrowseOnAction() {


        FileChooser fileChooserShares = new FileChooser();
        fileChooserShares.setTitle("Select data file .csv");
        fileChooserShares.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("CSV Files", "*.csv")
        );

        File selectedFile = fileChooserShares.showOpenDialog(Main.stage);
        if (String.valueOf(selectedFile).equals("null")) {

            return;
        }
        else{
            textFieldPathFile.setText(selectedFile.toString());

        }
    }


    @FXML
    void btnGenerateGraphOnAction() throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource("Graph.fxml"));
        Stage stage = new Stage();
        Scene scene = new Scene(fxmlLoader.load(), 1240, 816);
        stage.setTitle("Generations VS fitness");
        stage.setScene(scene);
        stage.show();
    }


    @FXML
    void btnReadOnAction() {

        String URLPath;
        try {
            if(textFieldPathFile.getText().equals("")) {
               throw new Exception();
            }else {
                URLPath=textFieldPathFile.getText();
                File file = new File(URLPath);

               fileReading(file);
            }
        }catch (Exception e) {
           JOptionPane.showMessageDialog(null, "Please browse a file");

        }


    }

    public void fileReading(File fName) {

        Scanner userText = null;
        TimeSlot.generateTimeSlots();
       Courses.clear();
       doctorsNameSet.clear();
       coursesNameSet.clear();

        try {
            userText = new Scanner(fName);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        assert userText != null;
        userText.nextLine();
        while (userText.hasNextLine()) {
            String userLine = userText.nextLine();
            userLine = userLine.replaceAll("[\n\t]", "");
            if (userLine.equals("")) {
                continue;
            }
            String[] courseSlice;
            courseSlice = userLine.split(",");

            Course c;
            if(courseSlice[0].trim().charAt(5)=='1'){
                c= new Course(courseSlice[0].trim(),'l', courseSlice[1].trim(), courseSlice[2].trim(), courseSlice[3].trim());

            }else {
                c= new Course(courseSlice[0].trim(),'c', courseSlice[1].trim(), courseSlice[2].trim(), courseSlice[3].trim());
            }
               Courses.add(c);
               doctorsNameSet.add(courseSlice[2].trim());
               coursesNameSet.add(courseSlice[3].trim());
               coursesCode.add(courseSlice[0].trim());
        }

        tablesView.setDoctorsNames(doctorsNameSet);
        tablesView.setCoursesNames(coursesNameSet);

        GeneticAlgorithm.SolveGenetic();
        for (int i=0;i<Courses.size();i++){

            int TimeSlotID =GeneticAlgorithm.population.get(0).chromosome[i];
            tablesView.courses=Courses;
            tablesView.courses.get(i).timeSlot= TimeSlot.AllTimeSlots.get(TimeSlotID);
        }

        tablesView.fillAllTables();


    }



    public static void main(String[] args) {

        launch();

    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {


        FXMLLoader fxmlLoader2 = new FXMLLoader(Main.class.getResource("Tables.fxml")) ;
        BorderPane anchorPane ;
        try {
            anchorPane = fxmlLoader2.load();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        tablesView = fxmlLoader2.getController();

        Scroll_pane.setContent(anchorPane);
    }
}
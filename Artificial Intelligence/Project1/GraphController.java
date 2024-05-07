package com.example.ai_project;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Label;

import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

public class GraphController implements Initializable {

    @FXML
    private Label BestFitness;

    @FXML
    private LineChart<String, Number> Chart;

    @FXML
    private Label NumberOfGenerations;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

       XYChart.Series<String, Number> Data = new XYChart.Series<>();
        int m = 1;
        for (; m < GeneticAlgorithm.Generations.size(); m++) {
            if (m % 17 == 0) {
                Data.getData().add(new XYChart.Data<>(String.valueOf(GeneticAlgorithm.Generations.get(m)), GeneticAlgorithm.Fitness.get(m)));

            }
        }
        BestFitness.setText(String.valueOf(GeneticAlgorithm.Fitness.get(GeneticAlgorithm.Fitness.size()-1)));
       NumberOfGenerations.setText(String.valueOf(GeneticAlgorithm.Generations.get(GeneticAlgorithm.Generations.size()- 1)));
        Chart.getData().add(Data);
        Chart.lookup(".chart-plot-background").setStyle("-fx-background-color: white");
        Data.getNode().setStyle("-fx-stroke: #E4D2F0;");


    }
}

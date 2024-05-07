package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.FileChooser;

import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    @FXML
    private TextField textFieldPathFile;

    @FXML
    private Spinner<?> spinnerTimeQuantaum;

    @FXML
    private Spinner<?> spinnerMemorySize;

    @FXML
    private RadioButton radioBtnSecondChance;

    @FXML
    private RadioButton radioBtnClock;

    @FXML
    private TableView<?> tabelView1;

    @FXML
    private TableColumn<?, ?> pId;

    @FXML
    private TableColumn<?, ?> numberOfFaults;

    @FXML
    private TableColumn<?, ?> start;

    @FXML
    private TableColumn<?, ?> finish;

    @FXML
    private TableColumn<?, ?> turnAround;

    @FXML
    private TableColumn<?, ?> waitTime;

    @FXML
    private TableView<?> tabelView2;

    @FXML
    private TableColumn<?, ?> traces;

    @FXML
    private TableColumn<?, ?> process;

    @FXML
    private TableColumn<?, ?> trueFalse;

    @FXML
    private TextField textFieldTotalCycles;

    @FXML
    private TextField textFieldTotalFaults;

    @FXML
    void btnBrowseOnAction(ActionEvent event) {

        FileChooser fileChooserShares = new FileChooser();
        fileChooserShares.setTitle("Select data file .txt");
        fileChooserShares.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("Text Files", "*.txt"),

                new FileChooser.ExtensionFilter("TXT files (' txt')", "*.txt")

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
    void btnReadOnAction(ActionEvent event) {



    }

    @FXML
    void radioBtnSecondChanceOnAction(ActionEvent event) {



    }
    
    @FXML
    void radioBtnClockOnAction(ActionEvent event) {



    }
    
    @FXML
    void btnRunOnAction(ActionEvent event) {

    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

        disableTextFiled();

    }

    private void disableTextFiled(){

        textFieldPathFile.setEditable(false);
        textFieldTotalFaults.setEditable(false);
        textFieldTotalCycles.setEditable(false);

    }
}

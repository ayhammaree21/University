module com.example.ai_project {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens com.example.ai_project to javafx.fxml;
    exports com.example.ai_project;
}
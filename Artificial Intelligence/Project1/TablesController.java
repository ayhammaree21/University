package com.example.ai_project;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;

import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.ResourceBundle;
import java.util.Set;

public class TablesController implements Initializable {


    public HashMap<String, String> mappingTimeStart = new HashMap<>();
    public HashMap<String, String> mappingTimeEnd = new HashMap<>();
    public ArrayList<Course> courses;

    @FXML
    private ComboBox<String> ComboBoxDoctors;

    @FXML
    private ComboBox<String> ComboBoxCourses;

    @FXML
    private TableColumn<ResultTable, String> CourseCode;

    @FXML
    private TableColumn<ResultTable, String> CourseCode1;

    @FXML
    private TableColumn<ResultTable, String> CourseCode2;

    @FXML
    private TableColumn<ResultTable, String> CourseCode11;

    @FXML
    private TableColumn<ResultTable, String> CourseCode111;

    @FXML
    private TableColumn<ResultTable, String> CourseCode1111;

    @FXML
    private TableColumn<ResultTable, String> CourseName;

    @FXML
    private TableColumn<ResultTable, String> CourseName1;

    @FXML
    private TableColumn<ResultTable, String> CourseName2;

    @FXML
    private TableColumn<ResultTable, String> CourseName11;

    @FXML
    private TableColumn<ResultTable, String> CourseName111;

    @FXML
    private TableColumn<ResultTable, String> CourseName1111;

    @FXML
    private TableColumn<ResultTable, String> Days;

    @FXML
    private TableColumn<ResultTable, String> Days1;

    @FXML
    private TableColumn<ResultTable, String> Days2;

    @FXML
    private TableColumn<ResultTable, String> Days11;

    @FXML
    private TableColumn<ResultTable, String> Days111;

    @FXML
    private TableColumn<ResultTable, String> Days1111;

    @FXML
    private TableView<ResultTable> FifthTableView;

    @FXML
    private TableView<ResultTable> FourthTableView;

    @FXML
    private TableColumn<ResultTable, String> InstructorName;

    @FXML
    private TableColumn<ResultTable, String> InstructorName1;

    @FXML
    private TableColumn<ResultTable, String> InstructorName11;

    @FXML
    private TableColumn<ResultTable, String> InstructorName111;

    @FXML
    private TableColumn<ResultTable, String> InstructorName1111;

    @FXML
    private TableColumn<ResultTable, String> InstructorName2;

    @FXML
    private TableView<ResultTable> SecondTableView;

    @FXML
    private TableColumn<ResultTable, String> Section;

    @FXML
    private TableColumn<ResultTable, String> Section1;

    @FXML
    private TableColumn<ResultTable, String> Section11;

    @FXML
    private TableColumn<ResultTable, String> Section111;

    @FXML
    private TableColumn<ResultTable, String> Section1111;

    @FXML
    private TableColumn<ResultTable, String> Section2;

    @FXML
    private TableView<ResultTable> DoctorsTableView;

    @FXML
    private TableView<ResultTable> CourseTableView;

    @FXML
    private TableView<ResultTable> ThirdTableView;

    @FXML
    private TableColumn<ResultTable, String> Time;

    @FXML
    private TableColumn<ResultTable, String> Time1;

    @FXML
    private TableColumn<ResultTable, String> Time2;

    @FXML
    private TableColumn<ResultTable, String> Time11;

    @FXML
    private TableColumn<ResultTable, String> Time111;

    @FXML
    private TableColumn<ResultTable, String> Time1111;

    public ObservableList<ResultTable> ListForDoctors = FXCollections.observableArrayList();
    public ObservableList<ResultTable> ListForCourses = FXCollections.observableArrayList();
    public ObservableList<ResultTable> ListForSecond = FXCollections.observableArrayList();
    public ObservableList<ResultTable> ListForThird = FXCollections.observableArrayList();
    public ObservableList<ResultTable> ListForFourth = FXCollections.observableArrayList();
    public ObservableList<ResultTable> ListForFifth = FXCollections.observableArrayList();

    @FXML
    void btnSelectDoctorOnAction() {

        String name = "";

        try {
            name = ComboBoxDoctors.getSelectionModel().getSelectedItem();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }

        if (!name.equals("")) {
            ListForDoctors.clear();
            for (Course c : courses) {
                if (c.instructorName.equals(name)) {
                    ResultTable t = new ResultTable(c.courseCode, c.courseName, c.instructorName, c.section, c.timeSlot.getDayString() + " ", mappingTimeStart.get(c.timeSlot.getStartTime() + "") + " - " + mappingTimeEnd.get(c.timeSlot.getEndTime() + ""));
                    ListForDoctors.add(t);
                }

            }

            DoctorsTableView.setItems(ListForDoctors);
        } else {
            ListForDoctors.clear();
        }

    }

    @FXML
    void btnSelectCourseOnAction() {

        String name = "";

        try {
            name = ComboBoxCourses.getSelectionModel().getSelectedItem();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }

        ListForCourses.clear();
        if (!name.equals("")) {
            for (Course c : courses) {
                if (c.courseName.equals(name)) {
                    ResultTable t = new ResultTable(c.courseCode, c.courseName, c.instructorName, c.section, c.timeSlot.getDayString() + " ", mappingTimeStart.get(c.timeSlot.getStartTime() + "") + " - " + mappingTimeEnd.get(c.timeSlot.getEndTime() + ""));
                    ListForCourses.add(t);
                }

            }

            CourseTableView.setItems(ListForCourses);
        }

    }

    void fillAllTables() {

        ListForSecond.clear();
        ListForThird.clear();
        ListForFourth.clear();
        ListForFifth.clear();

        for (Course c : courses) {

            ResultTable t = new ResultTable(c.courseCode, c.courseName, c.instructorName, c.section, c.timeSlot.getDayString() + " ", mappingTimeStart.get(c.timeSlot.getStartTime() + "") + " - " + mappingTimeEnd.get(c.timeSlot.getEndTime() + ""));
            if (c.courseCode.charAt(4) == '2') {
                ListForSecond.add(t);
            } else if (c.courseCode.charAt(4) == '3') {
                ListForThird.add(t);
            } else if (c.courseCode.charAt(4) == '4') {
                ListForFourth.add(t);
            } else if (c.courseCode.charAt(4) == '5') {
                ListForFifth.add(t);
            }

        }
        SecondTableView.setItems(ListForSecond);
        ThirdTableView.setItems(ListForThird);
        FourthTableView.setItems(ListForFourth);
        FifthTableView.setItems(ListForFifth);
    }

    public void setDoctorsNames(Set<String> teachersName) {
        ComboBoxDoctors.getItems().addAll(teachersName);
    }

    public void setCoursesNames(Set<String> coursesName) {
        ComboBoxCourses.getItems().addAll(coursesName);
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        CourseCode.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time.setCellValueFactory(new PropertyValueFactory<>("Time"));

        CourseCode1.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName1.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days1.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName1.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section1.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time1.setCellValueFactory(new PropertyValueFactory<>("Time"));

        CourseCode11.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName11.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days11.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName11.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section11.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time11.setCellValueFactory(new PropertyValueFactory<>("Time"));

        CourseCode111.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName111.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days111.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName111.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section111.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time111.setCellValueFactory(new PropertyValueFactory<>("Time"));

        CourseCode1111.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName1111.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days1111.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName1111.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section1111.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time1111.setCellValueFactory(new PropertyValueFactory<>("Time"));

        CourseCode2.setCellValueFactory(new PropertyValueFactory<>("CourseCode"));
        CourseName2.setCellValueFactory(new PropertyValueFactory<>("CourseName"));
        Days2.setCellValueFactory(new PropertyValueFactory<>("Days"));
        InstructorName2.setCellValueFactory(new PropertyValueFactory<>("InstructorName"));
        Section2.setCellValueFactory(new PropertyValueFactory<>("Section"));
        Time2.setCellValueFactory(new PropertyValueFactory<>("Time"));

        mappingTimeStart.put("8.0", "8:30");
        mappingTimeStart.put("9.5", "10:00");
        mappingTimeStart.put("11.0", "11:25");
        mappingTimeStart.put("12.5", "12:50");
        mappingTimeStart.put("14.0", "14:15");
        mappingTimeStart.put("15.5", "15:45");

        mappingTimeEnd.put("9.5", "9:45");
        mappingTimeEnd.put("11.0", "11:15");
        mappingTimeEnd.put("12.5", "12:40");
        mappingTimeEnd.put("14.0", "14:05");
        mappingTimeEnd.put("15.5", "15:30");
        mappingTimeEnd.put("17.0", "16:55");


    }


}

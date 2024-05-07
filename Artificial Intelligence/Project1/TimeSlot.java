package com.example.ai_project;
import java.util.ArrayList;
import java.util.Random;

public class TimeSlot {
    private static int count = 0;
    private final int id;
    private final double startTime;
    private final double endTime;
    private final String dayString;
    private final boolean[] days;

    public static ArrayList<TimeSlot> AllTimeSlots = null;


    public TimeSlot(double startTime, double endTime, boolean[] days, String daysString) {
        super();
        this.id=count;
        this.startTime = startTime;
        this.endTime = endTime;
        this.days = days;
        this.dayString = daysString;
        count++;
    }

    public static int getRandomTimeSlot(char courseType){

        Random r = new Random();
        int rand;
        if(courseType=='l'){
            rand= r.nextInt(15)+24;

        }else {
            rand= r.nextInt(24);

        }
          return rand;

    }

    public int getId() {
        return id;
    }

    public double getStartTime() {
        return startTime;
    }

    public double getEndTime() {
        return endTime;
    }

    public String getDayString() {
        return dayString;
    }

    public static double GreaterThanFourClasses(ArrayList<TimeSlot> timeSlots){
        double conflicts=0.0;

        int [] daysConflicts ={0,0,0,0,0};

        for (TimeSlot timeSlot : timeSlots) {

            for (int h = 0; h < 5; h++) {
                if (timeSlot.days[h]) {
                    daysConflicts[h]++;

                }
            }

        }
        for (int h = 0; h < 5; h++) {
            if (daysConflicts[h] >= 4) {
                conflicts+=1;

            }
        }

        return conflicts ;
    }


    public static double ReducingLabsAfterNoon(ArrayList<TimeSlot> timeSlots, ArrayList<Integer> sectionID){
        double conflicts = 0.0;
        for (int i = 0; i < timeSlots.size(); i++) {
            if(Main.Courses.get(sectionID.get(i)).courseType == 'l')
                continue;
            if (timeSlots.get(i).startTime == 14.0) {
                conflicts += 1;
            }
        }
        return conflicts ;
    }
    public static double DaysDistributionFitness(ArrayList<TimeSlot> timeSlots, ArrayList<Integer> sectionID) {
        double conflicts = 0.0;

            if (timeSlots.size() == 3) {   //Three sections

            int crossDay = 0;
            for (int h = 0; h < 5; h++) {
                if (timeSlots.get(0).days[h] || timeSlots.get(1).days[h] || timeSlots.get(2).days[h]) {
                    crossDay++;
                }
            }
            if (crossDay == 1) {

                conflicts += 3;

            }

            }
            else if (timeSlots.size() == 4) {  // Four sections
            int crossDay = 0;
            for (int h = 0; h < 5; h++) {
                if (timeSlots.get(0).days[h] || timeSlots.get(1).days[h] || timeSlots.get(2).days[h] || timeSlots.get(3).days[h]) {
                    crossDay++;
                }
            }

            if (crossDay == 1) { // Lab

                conflicts += 4;

            } else if (crossDay == 2) {
               conflicts+=2;
            }

        }
            else if (timeSlots.size() >= 5) {       // sections >=5  //lab
            int crossDay = 0;
            for (int h = 0; h < 5; h++) {
                for (TimeSlot timeSlot : timeSlots) {
                    if (timeSlot.days[h]) {
                        crossDay++;
                        break;
                    }
                }
            }
            if (crossDay <= 4) {
                conflicts += 5;
            }
        }

        return conflicts;
    }
    public static double checkDaysConflictForCourseSections(ArrayList<TimeSlot> timeSlots ){

        double conflicts = 0.0;

        for (int i = 0; i < timeSlots.size(); i++) {


            for (int j = i + 1; j < timeSlots.size(); j++){

                if (timeSlots.get(i).getId() == timeSlots.get(j).getId()) {  //Same day and time

                      conflicts+=3;

                } else if (timeSlots.get(i).getStartTime() == timeSlots.get(j).getStartTime()
                        &&  timeSlots.get(i).getEndTime() == timeSlots.get(j).getEndTime() ) {   // Same time diff days

                             conflicts+=1;

                }

            }
        }
        return conflicts;
    }



    public static double checkTimeConflictsForDoctors(ArrayList<TimeSlot> timeSlots) {
        double conflicts = 0.0;
        for (int i = 0; i < timeSlots.size(); i++) {
            for (int j = i + 1; j < timeSlots.size(); j++) {

                if (timeSlots.get(i).getId() == timeSlots.get(j).getId()) {
                    conflicts+=6;
                } else if (timeSlots.get(i).getStartTime() == timeSlots.get(j).getStartTime()
                        && timeSlots.get(i).getEndTime() == timeSlots.get(j).getEndTime()) {
                    for (int h = 0; h < 5; h++){
                        if (timeSlots.get(i).days[h] && timeSlots.get(j).days[h]){
                            conflicts+=6;
                            break;
                        }
                    }

                } else if ((timeSlots.get(i).getEndTime() - timeSlots.get(i).getStartTime() == 3
                        && timeSlots.get(j).getEndTime() - timeSlots.get(j).getStartTime() < 3)
                        || (timeSlots.get(i).getEndTime() - timeSlots.get(i).getStartTime() < 3
                        && timeSlots.get(j).getEndTime() - timeSlots.get(j).getStartTime() == 3)) {

                    boolean sameDay = false;
                    for (int h = 0; h < 5; h++) {
                        if (timeSlots.get(i).days[h] && timeSlots.get(j).days[h]) {
                            sameDay = true;
                            break;
                        }
                    }
                    if (sameDay) {
                        TimeSlot first = timeSlots.get(i);
                        TimeSlot second = timeSlots.get(j);
                        if (first.getStartTime() > second.getStartTime()) {
                            TimeSlot temp = first;
                            first = second;
                            second = temp;
                        }
                        if (first.getEndTime() > second.getStartTime()) {
                            conflicts+=6;
                        }

                    }

                }
            }
        }
        return conflicts;
    }




    public static void generateTimeSlots() {
        boolean[] SM = { true, true, false, false, false };
        boolean[] TR = { false, false, true, false, true };
        boolean[] SW = { true, false, false, true, false };
        boolean[] MW = { false, true, false, true, false };
        ArrayList<TimeSlot> coursesTimeSlots = new ArrayList<>();
        ArrayList<TimeSlot> labsTimeSlots = new ArrayList<>();
        AllTimeSlots=new ArrayList<>();
        for (double i = 8; i < 16; i += 1.5) {

            TimeSlot slot1 = new TimeSlot(i, i + 1.5, TR, "TR");
            TimeSlot slot2 = new TimeSlot(i, i + 1.5, SM, "SM");
            TimeSlot slot3 = new TimeSlot(i, i + 1.5, SW, "SW");
            TimeSlot slot4 = new TimeSlot(i, i + 1.5, MW, "MW");
            Slots(slot1, slot2, slot3, slot4, coursesTimeSlots);

        }

        for (int i = 8; i <= 14; i += 3) {
            TimeSlot labSlot1 = new TimeSlot(i , i + 3, new boolean[] { true, false, false, false, false }, "S");
            TimeSlot labSlot2 = new TimeSlot(i, i + 3, new boolean[] { false, true, false, false, false }, "M");
            TimeSlot labSlot3 = new TimeSlot(i, i + 3, new boolean[] { false, false, true, false, false }, "T");
            TimeSlot labSlot4 = new TimeSlot(i, i + 3, new boolean[] { false, false, false, true, false }, "W");
            TimeSlot labSlot5 = new TimeSlot(i, i + 3, new boolean[] { false, false, false, false, true }, "R");
            Slots(labSlot1, labSlot2, labSlot3, labSlot4, labsTimeSlots);
            labsTimeSlots.add(labSlot5);
            AllTimeSlots.add(labSlot5);
        }

    }

    private static void Slots(TimeSlot labSlot1, TimeSlot labSlot2, TimeSlot labSlot3, TimeSlot labSlot4, ArrayList<TimeSlot> labsTimeSlots) {
        labsTimeSlots.add(labSlot1);
        AllTimeSlots.add(labSlot1);
        labsTimeSlots.add(labSlot2);
        AllTimeSlots.add(labSlot2);
        labsTimeSlots.add(labSlot3);
        AllTimeSlots.add(labSlot3);
        labsTimeSlots.add(labSlot4);
        AllTimeSlots.add(labSlot4);
    }


}

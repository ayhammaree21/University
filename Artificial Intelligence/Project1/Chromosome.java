package com.example.ai_project;
import java.util.ArrayList;

public class Chromosome implements Comparable<Chromosome> {

    public int[] chromosome = new int[Main.Courses.size()];
    public double totalFitness;


    public Chromosome(boolean random) {
        if (random) {
            for (int i = 0; i < chromosome.length; i++) {
                this.chromosome[i] = TimeSlot.getRandomTimeSlot(Main.Courses.get(i).courseType);
            }
        }

    }


    public Chromosome(Chromosome c) {
        System.arraycopy(c.chromosome, 0, this.chromosome, 0, c.chromosome.length);

    }


    public void CalculateTotalFitness() {
        this.totalFitness = CalculateSectionsConflict() + CalculateDoctorsConflicts();
    }


    public double CalculateSectionsConflict() {
        double doctorsConflicts = 0.0;

        Object[] Codes = Main.coursesCode.toArray();
        for (int d = 0; d < Main.coursesCode.size(); d++) {

            ArrayList<TimeSlot> timeSlotsForSections = new ArrayList<>();
            ArrayList<Integer> sectionID = new ArrayList<>();
            String code = (String) Codes[d];

            for (int i = 0; i < this.chromosome.length; i++) {

                if (Main.Courses.get(i).courseCode.equals(code)) {
                    timeSlotsForSections.add(TimeSlot.AllTimeSlots.get(this.chromosome[i]));
                    sectionID.add(i);
                }

            }
            double conflect = TimeSlot.checkDaysConflictForCourseSections(timeSlotsForSections)
                            + TimeSlot.DaysDistributionFitness(timeSlotsForSections, sectionID)
                            + TimeSlot.ReducingLabsAfterNoon(timeSlotsForSections, sectionID);
            doctorsConflicts += conflect;
        }

        return doctorsConflicts;
    }


    public double CalculateDoctorsConflicts() {
        double doctorsConflicts = 0.0;


        Object[] arrDoctors = Main.doctorsNameSet.toArray();

        for (int d = 0; d < Main.doctorsNameSet.size(); d++) {
            ArrayList<TimeSlot> timeSlotsForDoctor = new ArrayList<>();
            String doctorName = (String) arrDoctors[d];
            for (int i = 0; i < this.chromosome.length; i++) {

                if (Main.Courses.get(i).instructorName.equals(doctorName)) {

                    timeSlotsForDoctor.add(TimeSlot.AllTimeSlots.get(chromosome[i]));
                }

            }
            double conflict = TimeSlot.checkTimeConflictsForDoctors(timeSlotsForDoctor) + TimeSlot.GreaterThanFourClasses(timeSlotsForDoctor);
            doctorsConflicts += conflict;

        }

        return doctorsConflicts;
    }


    @Override
    public int compareTo(Chromosome compare) {
        int compareFit = (int) compare.totalFitness;

        return (int) (this.totalFitness - compareFit);

    }
}

package com.example.ai_project;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class GeneticAlgorithm {

    public static int initialPopulationSize = 500;
    public static double crossOverRate = 0.6;
    public static double mutationRate = 0.3;
    public static int maxPopulationSize = 1000;
    public static int maxIterations =16000;
    public static double bestFitness = 3000;
    public static Chromosome best = null;

    public static ArrayList<Chromosome> population = new ArrayList<>();


    public static void createPopulation() {

        population.clear();

        for (int i = 0; i < initialPopulationSize; i++) {
            Chromosome chromosome = new Chromosome(true);
            population.add(chromosome);
            chromosome.CalculateTotalFitness();

        }

    }

    public static ArrayList<Chromosome> crossOverByCut(Chromosome parent1, Chromosome parent2, Chromosome child1, Chromosome child2) {

        Random r = new Random();
        int cut1 = r.nextInt(0, Main.Courses.size());
        int cut2 = r.nextInt(0, Main.Courses.size());

        if (cut2 < cut1) {
            int save = cut1;
            cut1 = cut2;
            cut2 = save;
        }
        for (int i = 0; i < cut1; i++) {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }
        for (int i = cut1; i < cut2; i++) {
            child1.chromosome[i] = parent2.chromosome[i];
            child2.chromosome[i] = parent1.chromosome[i];
        }
        for (int i = cut2; i < parent1.chromosome.length; i++) {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }

        ArrayList<Chromosome> child = new ArrayList<>();
        child.add(child1);
        child.add(child1);
        return child;
    }

    public static ArrayList<Chromosome> crossOverByRandom(Chromosome parent1, Chromosome parent2, Chromosome child1, Chromosome child2) {

        for (int i = 0; i < Main.Courses.size(); i++) {

            if (Math.random() < 0.5) {
                child1.chromosome[i] = parent1.chromosome[i];
                child2.chromosome[i] = parent2.chromosome[i];
            } else {
                child1.chromosome[i] = parent2.chromosome[i];
                child2.chromosome[i] = parent1.chromosome[i];
            }

        }
        ArrayList<Chromosome> child = new ArrayList<>();
        child.add(child1);
        child.add(child1);
        return child;
    }


    public static ArrayList<Chromosome> crossOver(Chromosome parent1, Chromosome parent2) {
        Chromosome child1 = new Chromosome(false);
        Chromosome child2 = new Chromosome(false);
        ArrayList<Chromosome> children;

        if (Math.random() < 0.5) {
            children = crossOverByCut(parent1, parent2, child1, child2);

        } else {
            children = crossOverByRandom(parent1, parent2, child1, child2);
        }

        children.get(0).CalculateTotalFitness();
        children.get(1).CalculateTotalFitness();

        return children;
    }

    public static Chromosome mutationByGene(Chromosome parent) {
        Chromosome child = new Chromosome(parent);
        Random r = new Random();
        int gen = r.nextInt(0, child.chromosome.length);
        child.chromosome[gen] = TimeSlot.getRandomTimeSlot(Main.Courses.get(gen).courseType);
        child.CalculateTotalFitness();
        return child;
    }

    public static ArrayList<Double> Fitness = new ArrayList<>();
    public static ArrayList<Integer> Generations = new ArrayList<>();

    public static void SolveGenetic() {
        population.clear();
        createPopulation();
        int i;
        for (i=0; i <= maxIterations; i++) {

            Collections.sort(GeneticAlgorithm.population);

            if (population.size() > maxPopulationSize) {
                int removeRange= population.size()-maxPopulationSize;
                for (int r=0;r<removeRange;r++){
                    population.remove( population.size()-1);
                }
            }

            ArrayList<Chromosome> offspring;

            Chromosome parent1 = population.get(0);
            Chromosome parent2 = population.get(1);

            if (Math.random() < crossOverRate) {
                offspring = crossOver(parent1, parent2);
                if (offspring.get(0).totalFitness < bestFitness) {
                    bestFitness = offspring.get(0).totalFitness;
                    best = offspring.get(0);

                }
                if (offspring.get(1).totalFitness < bestFitness) {
                    bestFitness = offspring.get(1).totalFitness;
                    best = offspring.get(1);

                }
                    population.add(offspring.get(0));
                    population.add(offspring.get(1));


            }
            if (Math.random() < mutationRate) {
                Chromosome child = mutationByGene(population.get(0));
                if (child.totalFitness < bestFitness) {
                    bestFitness = child.totalFitness;
                    best = child;
                }
                population.add(child);
            }



            System.out.println("Iteration Number: "+i);
            System.out.println("Best Fitness:  " + (bestFitness));

            Generations.add(i);
            Fitness.add(bestFitness);


        }

    }

}







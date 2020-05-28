package bearmaps.utils.graph;
import bearmaps.utils.graph.AStarGraph;
import bearmaps.utils.graph.ShortestPathsSolver;
import bearmaps.utils.graph.SolverOutcome;
import bearmaps.utils.pq.DoubleMapPQ;
import edu.princeton.cs.algs4.Stopwatch;
import org.junit.rules.Timeout;

import java.util.*;

public class AStarSolver<Vertex> implements ShortestPathsSolver<Vertex>{

    private SolverOutcome outcome;
    private List<Vertex> solution;
    private double solutionWeight;
    private int numStatesExplored;
    private double explorationTime;

    public AStarSolver(AStarGraph<Vertex> input, Vertex start, Vertex end, double timeout) {
        Stopwatch timer = new Stopwatch();
        numStatesExplored = 0;
        solution = new LinkedList<>();
        DoubleMapPQ<Vertex> thePriorityQ = new DoubleMapPQ<>();
        HashMap<Vertex, Double> distanceTothis = new HashMap<>();
        HashMap<Vertex,Vertex> pred = new HashMap<>();
        thePriorityQ.insert(start, input.estimatedDistanceToGoal(start, end));
        distanceTothis.put(start, 0.0);
        pred.put(start,null);

        while (!thePriorityQ.peek().equals(null) && !thePriorityQ.peek().equals(end) && !(explorationTime > timeout)) {
            Vertex current = thePriorityQ.poll();
            System.out.print(current);
            for (WeightedEdge<Vertex> edge : input.neighbors(current)) {
                if(distanceTothis.get(edge.to()) == null){
                    distanceTothis.put(edge.to(), distanceTothis.get((current))+ edge.weight());}
                relax(edge, thePriorityQ, distanceTothis, input, end, pred);
            }

            numStatesExplored += 1;
            explorationTime = timer.elapsedTime();
        }
        if (explorationTime > timeout ){
            outcome = SolverOutcome.TIMEOUT;
        }
        if (thePriorityQ.peek().equals(end)){
            outcome = SolverOutcome.SOLVED;
            Vertex current = end;
            solution.add(end);
            while (pred.get(current) != null) {
                current = pred.get(current);
                solution.add(current);
            }
//            solution.add(start);
//            ArrayList<Vertex> change = new ArrayList<>();
//            for (Vertex i : solution){
//                change.add(i);
//            }
            Collections.reverse(solution);
//            solution = change;
        }

        if (thePriorityQ.peek().equals(null)){
            outcome = SolverOutcome.UNSOLVABLE;
        }
    }
    class The_Comparator implements Comparator<Double> {
        @Override
        public int compare(Double o1, Double o2) {
            if (o1-o2>1) {
                return 1;
            }
            if (o1 -o2 <0){
                return -1;
            }
            return 0;
        }
    }
    public void relax(WeightedEdge<Vertex> edge , DoubleMapPQ<Vertex> thePriorityQ, HashMap<Vertex, Double> distanceTothis, AStarGraph<Vertex> input,Vertex goal, HashMap<Vertex,Vertex> pred ){
        Vertex p = edge.from();
        Vertex q = edge.to();
        Double w = edge.weight();
        if((distanceTothis.get(p)+ w) <= distanceTothis.get(q)){
            distanceTothis.put(q, distanceTothis.get(p)+ w);
            pred.put(q,p);
            if (thePriorityQ.contains(q)) {
                thePriorityQ.changePriority(q, distanceTothis.get(q) + input.estimatedDistanceToGoal(q, goal));
            }
            if(!thePriorityQ.contains(q)) {
                thePriorityQ.insert(q, distanceTothis.get(q) + input.estimatedDistanceToGoal(q, goal));
            }
        }
    }

    @Override
    public SolverOutcome outcome() {
        return this.outcome;
    }

    @Override
    public List<Vertex> solution() {
        return  solution;
    }

    @Override
    public double solutionWeight() {

        return this.solutionWeight;

    }

    @Override
    public int numStatesExplored() {

        return this.numStatesExplored;

    }

    @Override
    public double explorationTime() {
        return this.explorationTime;
    }
}
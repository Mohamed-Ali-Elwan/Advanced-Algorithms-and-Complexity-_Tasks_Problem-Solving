import java.util.*;
public class computer_game {
    static int spot_size=8;
    static int target_position;
    static boolean target_hited =false;
    static int shots_count = 0;
    public static int move_target(int move_left, int move_right) {
        if (move_left < 1) {
            return move_right;
        }
        else if (move_right > spot_size) {
            return move_left;
        }
        else {
            return (new Random().nextInt(2)==0) ? move_left :move_right ;
        }
    }
    public static void DivideConqershot(int left, int right ) {
        shots_count++;
        if (left > right) {
            shots_count--;
            return;
        }
        int mid = (left + right) / 2;
        if (mid == target_position) {
            System.out.println("success : Shot at: " + mid + ", Target at: " + target_position);
            System.out.println("Hit at ==>> " + mid);
            target_hited = true;
            return;
        }
        if (!target_hited) {
            System.out.println("Failed : Shot at: " + mid + ", Target at: " + target_position);
            target_position = move_target(target_position - 1, target_position + 1);
            DivideConqershot(left, mid - 1);

        }
        if (!target_hited) {
//            System.out.println("Failed : Shot at: " + mid + ", Target at: " + target_position);
//            target_position = move_target(target_position - 1, target_position + 1);
            DivideConqershot(mid + 1, right);
        }
        return;
    }

    public static int greedy_shot() {
        int shotat  = 2;
        int shotDirection  = 1;
        int count = spot_size - 2;
        int NumberOfItration=1;
        while (!target_hited) {
            shots_count++;
            if (shotat  == target_position) {
                System.out.println("success : Shot at: " + shotat + ", Target at: " + target_position);
                System.out.println("Hit at ==>> " + shotat );
                target_hited = true;
                break;
            }
            System.out.println("Failed : Shot at: " + shotat  + ", Target at: " + target_position);
            target_position = move_target(target_position - 1, target_position + 1);
            count--;
            if (count == 0) {
                shotDirection  = -1;
                count = spot_size - 2;
                NumberOfItration++;
            }
            else {
                shotat  += shotDirection ;
            }
        }
        return NumberOfItration;
    }

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int algoId ;
        System.out.println("Select Algorithm");
        System.out.println("Algorithm  ==>  ID");
        System.out.println("Divide & Conquer ==> 1 ");
        System.out.println("Greedy ==> 2 ");
        System.out.print
                ("Enter Algorithm ID: ");
        algoId = input.nextInt();
        System.out.print("Enter number of spots: ");
        spot_size = input.nextInt();
        target_position = (new Random().nextInt(spot_size) + 1);
        if(algoId==1){
        int NumberOfItration = 1;
        while (!target_hited) {
            DivideConqershot(1, spot_size);
            if (!target_hited)
                NumberOfItration++;
        }
        System.out.println("Shot counts ==>> " + shots_count);
        System.out.println("number of itration time  ==>> " + NumberOfItration);
    }
        else if (algoId==2){
           int NumberOfItration=greedy_shot();
            System.out.println("Shot counts ==>> " + shots_count);
            System.out.println("number of itration time  ==>> " + NumberOfItration);
        }
        else {
            System.out.println("false input");
            System.out.println("select Algorithm ID (1 or 2)");
            System.out.print("Divide & Conquer ==> 1 ");
            System.out.print("Greedy ==> 2 ");
        }
        if (!target_hited)
            System.out.println("The algorithm failed to find the target.");
        System.out.println("-----------------------------------------------------------------------");
    }
}

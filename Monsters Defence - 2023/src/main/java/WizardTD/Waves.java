package WizardTD;

import java.util.ArrayList;

public class Waves {
    public int duration;
    public double pre_wave_pause;
    public ArrayList<Monster> monsters;
    public int timer = 0;
    public ArrayList<Monster> monsters_final;
    public int monster_increment = 0;

    public Waves(int duration, double pre_wave_pause, ArrayList<Monster> monsters) {
        this.duration = duration*60;
        this.pre_wave_pause = pre_wave_pause*60;
        this.monsters = monsters;
        this.monsters_final = new ArrayList<>();
    }
}

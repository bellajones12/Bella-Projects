package WizardTD;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;


public class ManaTest {
    Mana mana = new Mana(50, 100);

    @Test
    public void constructor() {
        assertNotNull(mana);
    }

    @Test
    public void getManaTest() {
        assertEquals(50, mana.getMana());
    }

    @Test
    public void changeManaTest() {
        mana.changeMana(5);
        assertEquals(55, mana.getMana());
    }

    @Test
    public void changeManaCapTest() {
        mana.changeMana(60);
        assertEquals(100, mana.getMana());
    }



}
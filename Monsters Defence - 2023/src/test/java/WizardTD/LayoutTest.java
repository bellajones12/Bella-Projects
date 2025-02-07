package WizardTD;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class LayoutTest{
    Layout layout = new Layout(760, 680, "level1.txt", null, null, null, null, null, null, null, null, null, null, null, null, null, null);

    @Test
    public void constructor() {
        assertNotNull(layout);
    }

    @Test
    public void testArray() {
        layout.readMap();
        assertNotNull(layout.array);
    }

    @Test
    public void readMapTest() {
        assertEquals(680, layout.mapHeight);
        assertEquals(760, layout.mapWidth);
    }
}

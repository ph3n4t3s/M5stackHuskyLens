#include "TestBase.h"
#include "../src/display/DisplayManager.h"

class DisplayTest : public TestBase {
protected:
    static DisplayManager display;
public:
    static void runAllTests() {
        RUN_TEST(testInit);
        RUN_TEST(testMessage);
        RUN_TEST(testClear);
    }
    
private:
    static void testInit() {
        printTestInfo("Display Initialization");
        checkMemory("Before Init");
        
        display.begin();
        TEST_ASSERT_TRUE(M5.Lcd.width() == Constants::SCREEN_WIDTH);
        TEST_ASSERT_TRUE(M5.Lcd.height() == Constants::SCREEN_HEIGHT);
        
        checkMemory("After Init");
    }
    
    static void testMessage() {
        printTestInfo("Display Message");
        
        String testMsg = "Test Message";
        display.showMessage(testMsg);
        TEST_ASSERT_EQUAL(Constants::Colors::TEXT, M5.Lcd.textcolor);
        
        delay(2000); // Pour voir le message
        display.clearScreen();
    }
    
    static void testClear() {
        printTestInfo("Display Clear");
        
        display.clearScreen();
        // Vérification visuelle nécessaire
        delay(1000);
        
        display.showMessage("Screen should be clear");
        delay(2000);
    }
};

DisplayManager DisplayTest::display;

void setup() {
    UNITY_BEGIN();
    DisplayTest::runAllTests();
    UNITY_END();
}

void loop() {
    delay(1000);
}
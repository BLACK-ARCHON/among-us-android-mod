package uk.lgl.modmenu;

import android.content.Context;
import android.content.SharedPreferences;

public class Preferences {
    private static SharedPreferences.Editor editor;
    public static Context context;
    public static boolean isSoundEnabled = true, savePref = false;

    public static native void Changes(Context context, int feature, int value, boolean bool, String str);

    public static void changeFeatureInt(String feature, int featureNum, int value) {
        Changes(context, featureNum, value, false, feature);
        editor.putInt(feature, value).apply();
    }

    public static void changeFeatureBoolean(String feature, int featureNum, boolean value) {
        if (featureNum == 1000)
            isSoundEnabled = value;
        if (featureNum == 1001)
            savePref = value;
        Changes(context, featureNum, 0, value, feature);

        editor.putBoolean(feature, value).apply();
    }

    //TODO: changeFeatureString

    public static int loadPrefInt(String feature) {
        if (savePref) {
            SharedPreferences preferences = context.getSharedPreferences("mod_menu", 0);
            editor = preferences.edit();
            return preferences.getInt(feature, 0);
        }
        return 0;
    }

    public static boolean loadPrefBoolean(String feature) {
        if (savePref || (feature == "Sounds" || feature == "Save preferences")) {
            SharedPreferences preferences = context.getSharedPreferences("mod_menu", 0);
            editor = preferences.edit();
            if (!preferences.contains("Sounds"))
                return true;
            return preferences.getBoolean(feature, false);
        }
        return false;
    }
}

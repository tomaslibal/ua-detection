#include <string.h>

namespace Utils {

    bool in_array(char* array, char ch) {
        int i = 0;
        int len = strlen(array);
        for (; i < len; i++) {
            if (array[i] == ch) return true;
        }
        return false;
    }

}

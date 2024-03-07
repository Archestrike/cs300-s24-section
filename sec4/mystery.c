int func() {
    int max = 42;
    int sum = 0;
    for (int i = 0; i < max; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    return func();
}
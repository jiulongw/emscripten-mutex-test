# mutex deadlock test

## How to run
Make sure emsdk environment is ready. `emcc --version` should show emcc version.

1. Execute following command
    ```
    ./run
    ```
2. Open browser that supports `SharedArrayBuffer` (e.g. latest Chrome) and navigate to https://localhost:8080.

## What is the problem
This demonstrate that in WASM pthread, main thread that is in waiting stage can be re-entered. It will pause
the wait and start to execute work item submitted by `emscripten_async_run_in_main_runtime_thread`.

Note: Asyncify is not enabled in this program.

Here is a typical output of the program:
```
1: [16324] Enter main.
2: [16324] accquiring lock...
3: [16324] lock accquired.
4: [5266552] Enter worker_proc.
5: [16324] Enter run_in_main_thread.
6: [16324] accquiring lock...
7: [16324] lock accquired.
8: [16324] lock released.
9: [16324] lock released.
```

In line 3 main thread accuired the lock and started to sleep. In line 5 the same main thread started to do
something else. If the `recursive_mutex` in the code were replaced with `mutex`, the program will deadlock.

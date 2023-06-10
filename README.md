# Artha
> In Sanskrit: **अर्थ** =  one of main human life objectives: **wealth and prosperity**

Artha is a simple implementation of a cryptocurrency written from scratch. </br>
It is a proof-of-concept and cannot be regarded as stable.

* **Full** and **miner** nodes supported
* **Assymetric** key generation on secp256k1 elliptic curve
* **P2P** via websockets


<img src="./images/minernode.png" alt="Size Limit CLI" width="75%">

<details>

<summary> <h4> Send a transaction </h4> (screenshot) </summary>
<img src="./images/fullnode.png" alt="Size Limit CLI" width="75%">

</details>

# Compilation
   
* Download sources:
  ```bash
  git clone --recursive https://github.com/enclay/artha.git
  cd artha/
  ```

* Build with cmake:
  ```bash
  mkdir build && cd build
  cmake .. && make
  ./artha <command> [options]
  ```

#!/bin/bash

# Exécuter les scripts de test se trouvant dans le dossier shouldNotWork
for file in shouldNotWork/*.sh; do
    echo -e "\e[33mTesting $file"
    bash $file &
    pid=$!
    wait $pid
    if [ $? -eq 0 ]; then
        echo -e "\e[31m❌ Test failed: $file\e[0m"
        exit 1
    fi
        echo -e "\e[32m✅ Done testing $file\e[0m"
    sleep 0.2
done

# Exécuter les scripts de test se trouvant dans le dossier shouldWork
for file in shouldWork/*.sh; do
    echo -e "\e[33mTesting $file"
    bash $file &
    pid=$!
    wait $pid
    if [ $? -ne 0 ]; then
        echo -e "\e[31m❌ Test failed: $file\e[0m"
        exit 1
    fi
        echo -e "\e[32m✅ Done testing $file\e[0m"
    sleep 1.5
done
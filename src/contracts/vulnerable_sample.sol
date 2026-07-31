// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

/*
 * VULNERABLE BANK SMART CONTRACT
 * Purpose: Used as a test sample for the C++ static code analyzer.
 */
contract VulnerableBank {
    // Keeps track of how much Ether each user wallet has deposited
    mapping(address => uint256) public balances;

    /*
   * VULNERABILITY 1: Reentrancy Risk (C++ Scanner catches: .call{value:)
     
     * WHY IT MATTERS: 
     * The contract sends money OUT to the user BEFORE updating their balance. 
     * A malicious attacker can write a fallback function that "calls back" 
     * into this withdraw function recursively before the balance drops, 
     * draining the entire contract's funds.
     * 
     * SECURE FIX: Always update the balance state BEFORE sending Ether 
     * (Checks-Effects-Interactions pattern), or use a ReentrancyGuard modifier.
     */
    function withdraw(uint256 amount) public {
        require(balances[msg.sender] >= amount, "Insufficient balance");
        
        // [C++ SCANNER TARGET]: This line triggers the +40 risk score
        (bool sent, ) = msg.sender.call{value: amount}("");
        require(sent, "Failed to send Ether");

        balances[msg.sender] -= amount; // State change happens too late!
    }

    /*
     
     * VULNERABILITY 2: Dangerous Authorization (C++ Scanner catches: tx.origin)
     
     * WHY IT MATTERS:
     * 'tx.origin' points to the original wallet owner who initiated the transaction,
     * whereas 'msg.sender' is the immediate address calling the function.
     * Using 'tx.origin' for access control leaves the contract vulnerable to 
     * smart contract phishing attacks.
     * 
     * SECURE FIX: Always use 'msg.sender' for authentication checks instead of 'tx.origin'.
     */
    function sensitiveAction() public view returns (bool) {
        // [C++ SCANNER TARGET]: This line triggers the +30 risk score
        if (tx.origin != msg.sender) {
            return true;
        }
        return false;
    }
}

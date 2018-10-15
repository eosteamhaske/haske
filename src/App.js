import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import Eos from 'eosjs'

class App extends Component {
  render() {
    const eos = Eos({verbose:false, chainId: '038f4b0fc8ff18a4f0842a8f0564611f6e96e8535901dd45e43ac8691a1c4dca', keyProvider: ['5KJ91a7yNS2xtio1HPeLSm34xF8tu4BPVdtEeYqDSHCj9STrGNj'], httpEndpoint: 'http://dev.cryptolions.io:38888'})
    console.log(eos.getInfo())
    //eos.getCurrencyBalance("eos", "eltneg").then(console.log).catch(console.log)
    eos.getBlock('3000').then(console.log)
    //eos.hello('haskehelloc1', 'eosteamhaske')
    const options = {
      authorization: 'eosteamhaske@active',
      user: 'eosteamhaske',
      broadcast: true,
      sign: true
    }
    eos.contract('haskehelloc1').then(contract => contract.hi('eosteamhaske', options)).then(console.log)
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <p>
            eosteamhaske
          </p>
        </header>
      </div>
    );
  }
}

export default App;

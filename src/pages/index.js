import React from "react";
import { Route, BrowserRouter as Router, Switch } from "react-router-dom";

import Apply from './apply'
import Home from './home'
import LoanHistory from './loanHistory'
import Signup from './signup'
import CreditHistory from './creditHistory'
import LandingPage from './landingHome'

export default () => (
      <Router>
          <Switch>
            <Route exact path="/dashboard" component={Home} />
            <Route exact path="/apply" component={Apply} />
            <Route exact path="/loan-history" component={LoanHistory} />
            <Route exact path="/register" component={Signup} />
            <Route exact path="/credit-history" component={CreditHistory} />
            <Route exact path="/" component={LandingPage} />
          </Switch>
      </Router>
  );
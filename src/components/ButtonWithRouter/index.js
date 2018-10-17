import React from "react";

import { withRouter } from "react-router-dom";

const Button = props => {
  const { to, history, btnText } = props;
  return (
    <button
      onClick={e => {
        e.preventDefault();
        history.push(to);
      }}
    >
      {btnText}
    </button>
  );
};

export default withRouter(({ history, to, btnText }) => (
  <Button history={history} to={to} btnText={btnText} rest />
));

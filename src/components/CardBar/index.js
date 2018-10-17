import React from "react";
import './index.css';
import {Link} from 'react-router-dom'

export default () => (
  <div className="flex-container card-bar row">
    <Link className='a-link' to='/'><div>Haske</div></Link>
    <Link className='a-link' to='/credit-history'><div>Bal: 0 HSK</div></Link>
  </div>
);

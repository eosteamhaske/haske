import React from "react";
import './index.css';
import {Link} from 'react-router-dom'
import { Dropdown } from "semantic-ui-react";

const styles = {
  item: {
    fontSize: '8px'
  }
}

export default () => (
  <div className="flex-container card-bar row">
    <Link className='a-link' to='/'><div>Haske</div></Link>
    <Dropdown simple text="Balances">
      <Dropdown.Menu>
      <Dropdown.Item style={styles.item}>
      <Link className='a-link bal' to='/credit-history'><div>2 HSK</div></Link>
      </Dropdown.Item>
      <Dropdown.Item style={styles.item}>
      <Link className='a-link bal' to='/credit-history'><div>3 EOS</div></Link>
      </Dropdown.Item>
      <Dropdown.Item style={styles.item}>
      <Link className='a-link bal' to='/credit-history'><div>8 CNT</div></Link>
      </Dropdown.Item>
      </Dropdown.Menu>
    </Dropdown>
  </div>
);

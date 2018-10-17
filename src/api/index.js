import Eos from "eosjs";
import config from "./config";

const eos = Eos(config);
console.log(eos.getInfo());
eos.getBlock("3000").then(console.log);
const options = {
  authorization: "eosteamhaske@active",
  user: "eosteamhaske",
  broadcast: true,
  sign: true
};
eos
  .contract("haskehelloc1")
  .then(contract => contract.hi("eosteamhaske", options))
  .then(console.log);

const api = "api";

export default api;

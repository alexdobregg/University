const fs = require('fs');
var shopifyAPI = require('shopify-api-node');

var shopify = new shopifyAPI({
  shopName: 'shoeniverse-2836',
  accessToken:'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'
})

var requestType;
var productSku;
var valueToUpdate;

const readline = require('readline').createInterface({
  input: process.stdin,
  output: process.stdout
});

readline.question('Care este tipul requestului? Opțiuni: update & create_json.\n', option => {
  if (['update', 'create_json'].indexOf(option) != -1) {
    requestType = option;
  } else {
    console.log('Trebuie să alegi din opțiunile disponibile.');
    process.exit(0);
  }
  console.log(`Ai ales ${requestType}.`);
  if (requestType == 'update') {
    readline.question('Care este codul SKU al produsului?\n', option => {
      productSku = option;
      console.log(`Ai ales produsul cu SKU ${productSku}.`);
      readline.question('Care este noul preț al produslui? Acesta trebuie să fie cuprins între 0 si 10000.\n', option => {
        if (option >= 0 && option < 100000) {
          valueToUpdate = option;
        } else {
          console.log('Trebuie să alegi o valoare cuprinsă între 0 și 100000.');
          process.exit(0);
        }
        console.log(`Ai ales să modifici prețul produsului cu SKU ${productSku}, dându-i valoarea ${valueToUpdate}.`);
        
        readline.close();
      });
    });
  } else {
    readline.close();
  }
});

const createJSON = async function() {

  const products = await shopify.product.list();
  var integrationJSON = [];
  for (var product of products) {
    var itemJSON = {
      cod_unic: product.variants[0].sku,
      nume_producator: product.vendor,
      denumire_produs: product.title,
      categorie: product.product_type,
      pret: product.variants[0].price,
      url_produs: `https://shoeniverse-2836.myshopify.com/products/${product.handle}`,
    }
    for (var img of product.images) {
      itemJSON[`url_image${img.position}`] = img.src;
    }
    integrationJSON.push(itemJSON);
  }
  fs.writeFile('./all_products.json', JSON.stringify(integrationJSON), err => {
    if (err) {
      console.log('Eroare de scriere în fișier');
    }
    console.log('Fisierul json a fost generat cu succes.');
  });
};

const updateProduct = async function() {
  const products = await shopify.product.list();
  var variantIdx = '-1';
  for (var product of products) {
    for (var variant of product.variants) {
      if (variant.sku == productSku) {
        variantIdx = variant.id;
        break;
      }
    }
    if (variantIdx != '-1') {
      break;
    }
  }
  if (variantIdx == '-1') {
    console.log(`Produsul cu SKU ${productSku} nu există.`);
    process.exit(0);
  }
  await shopify.productVariant.update(variantIdx, {price: valueToUpdate});
  console.log('Produsul a fost modificat cu succes!');
}


readline.on('close', () => {
  if (requestType == 'update') {
    updateProduct();
  } else {
    createJSON();
  }
});

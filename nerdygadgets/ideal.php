<html>
<body style="display: none;">
    <form action="https://www.ideal-checkout.nl/payment/" method="post" id="form">
        <input name="gateway_code" type="hidden" value="ideal">
        <input name="order_id" type="hidden" value="<?= (isset($_GET['orderid'])) ? $_GET['orderid'] : "" ?>">
        <input name="order_description" type="hidden" value="Bestelling <?= (isset($_GET['orderid'])) ? $_GET['orderid'] : "" ?>">
        <input name="order_amount" type="hidden" value="<?= (isset($_GET['price'])) ? $_GET['price'] : "" ?>">
        <input name="url_success" type="hidden" value="http://localhost/nerdygadgets/order.php?succes=true&orderid=<?= $_GET['orderid'] ?>">
        <input name="url_pending" type="hidden" value="http://localhost/nerdygadgets/order.php?succes=true&opmerking=pending">
        <input name="url_cancel" type="hidden" value="http://localhost/nerdygadgets/order.php?succes=false&opmerking=cancel">
        <input name="url_error" type="hidden" value="http://localhost/nerdygadgets/order.php?succes=false">
        <input type="submit" value="Verder >>">
    </form>
</body>
</html>

<script>
    document.getElementById("form").submit();
</script>
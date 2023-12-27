'use client'
import {
    Alert,
    AlertIcon,
    AlertTitle,
    Card,
    Grid,
} from '@chakra-ui/react';
import { useSession } from 'next-auth/react';


export default function Page() {
    const { date: session } = useSession();
    const [title, setTitle] = useState('');
    const [description, setDescription] = useState('');
    const [destinationType, setDestinationType] = useState('')
    const [maxExpectedPrice, setMaxExpectedPrice] = useState(0);
    const [seekerExpiryDate, setSeekerExpiryDate] = useState('');

    const [isTitleInvalid, setIsTitleInvalid] = useState(false);
    const [isDescriptionInvalid, setIsDescriptionInvalid] = useState(false);
    const [isDestinationTypeInvalid, setIsDestinationTypeInvalid] = useState(false);
    const [isMaxExpectedPriceInvalid, setIsMaxExpectedPriceInvalid] = useState(false);
    const [isSeekerExpiryDateInvalid, setIsSeekerExpiryDateInvalid] = useState(false);

    const toast = useToast()
    const handleSubmit = async () => {
        if (title === '' || title.length > 20) {
            toast({
                title: '标题不合法',
                description: "标题不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsTitleInvalid(true);
            return;
        }
        if (description === '' || description.length > 10000) {
            toast({
                title: '描述不合法',
                description: "描述不能为空且长度不能超过10000",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsDescriptionInvalid(true);
            return;
        }
        if (destinationType === '' || destinationType.length > 20) {
            toast({
                title: '目的地类型不合法',
                description: "目的地类型不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsDestinationTypeInvalid(true);
            return;
        }
        if (maxExpectedPrice < 0 || maxExpectedPrice > 1000000) {
            toast({
                title: '最大期望价格不合法',
                description: "最大期望价格必须在0到1000000之间",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsMaxExpectedPriceInvalid(true);
            return;
        }
        if (seekerExpiryDate === '') {
            toast({
                title: '求购截止日期不合法',
                description: "求购截止日期不能为空",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsSeekerExpiryDateInvalid(true);
            return;
        }

        const res = await fetch(`${config.serverIp}/seekers`, {
            method: 'POST',
            body: JSON.stringify({
                seekerTitle: title,
                seekerDescription: description,
                destinationType: destinationType,
                maxExpectedPrice: maxExpectedPrice,
                seekerExpiryDate: seekerExpiryDate,
            }),
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
        })
        const request = await res.json()
        if (!res.ok || !request) {
            toast({
                title: '创建失败',
                description: `state: ${res.status}, message: ${response.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
        else {
            toast({
                title: '创建成功',
                description: '即将跳转至请求页面',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            const timer = setTimeout(() => {
                window.location.href = `/request?${request.seekerId}`
            }, 2000)

            return;
        }
    }



    return (
        <>
            {session ? (
                <>
                    <Card align='center' w='800px' h='500px'>
                        <CardHeader>
                            <Heading size='lg' >创建请求</Heading>
                        </CardHeader>

                        <CardBody>
                            <Grid templateRows='repeat(8, 1fr)' templateColumns='repeat(2, 1fr)' gap={1}>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>标题</Text>
                                    <Input
                                        value={title}
                                        onChange={(e) => setTitle(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isTitleInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>描述</Text>
                                    <Textarea
                                        w='300px'
                                        h='200px'
                                        value={description}
                                        onChange={(e) => setDescription(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isDescriptionInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>目的地类型</Text>
                                    <Input
                                        value={destinationType}
                                        onChange={(e) => setDestinationType(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isDestinationTypeInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>最大期望价格</Text>
                                    <Input
                                        value={maxExpectedPrice}
                                        onChange={(e) => setMaxExpectedPrice(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isMaxExpectedPriceInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>求购截止日期</Text>
                                    <Input
                                        value={seekerExpiryDate}
                                        onChange={(e) => setSeekerExpiryDate(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isSeekerExpiryDateInvalid}
                                    />
                                </GridItem>

                            </Grid>
                        </CardBody>
                        <CardFooter>
                            <Button colorScheme='telegram' size='lg' onClick={() => handleSubmit()} >提交</Button>
                        </CardFooter>
                    </Card>
                </>
            ) : (
                <Card align='center'>
                    <Alert status='error'>
                        <AlertIcon />
                        <AlertTitle>请先登录！</AlertTitle>
                    </Alert >
                </Card >
            )
            }
        </>
    );
}